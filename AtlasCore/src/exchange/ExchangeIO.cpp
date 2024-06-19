#pragma once
#include "pch.h"

#include "exchange/Exchange.hpp"
#include "exchange/ExchangeImpl.hpp"

BEGIN_ATLAS_NAMESPACE

//============================================================================
Result<bool> Exchange::initDir() noexcept {
  // get all the files in the directory
  fs::path path(m_impl->source);
  Vector<std::filesystem::path> files;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      files.push_back(entry.path());
    }
    if (entry.is_directory()) {
      String msg = "Exchange source directory contains subdirectories: " +
                   entry.path().string();
      return Err(msg);
    }
    if (entry.path().extension() != ".csv") {
      String msg = "Exchange source directory contains non-csv file: " +
                   entry.path().string();
      msg += ", found extension: " + path.extension().string() +
             ", expected: .csv";
      return Err(msg);
    }
    String asset_id = entry.path().stem().string();
    auto asset = Asset(asset_id, m_impl->assets.size());
    asset.setSource(entry.path().string());
    m_impl->assets.push_back(std::move(asset));
  }
  if (files.empty()) {
    return Err("Exchange source directory is empty");
  }

  // load the files
 Vector<std::thread> threads;
  String msg = "";
  std::mutex m_mutex;
  for (auto &asset : m_impl->assets) {
    threads.push_back(std::thread([this, &asset, &m_mutex, &msg]() {
      auto res = asset.loadCSV();
      if (!res) {
        std::lock_guard<std::mutex> lock(m_mutex);
        String error = res.error().what();
        String error_msg =
            std::format("Error loading asset: {} - {}\n", asset.id, error);
        msg += error_msg;
      }
    }));
  }

  // Join all threads
  for (auto &thread : threads) {
    thread.join();
  }

  if (!msg.empty()) {
    return Err(msg);
  }

  return true;
}

//============================================================================
Result<bool> Exchange::init() noexcept {
  // make sure the source string is file
  auto path = std::filesystem::path(m_impl->source);
  if (!std::filesystem::exists(path)) {
    return Err("Exchange source does not exist");
  }

  if (std::filesystem::is_directory(path)) {
    return initDir();
  }

#ifdef ATLAS_HDF5
  // make sure the source file is an HDF5 file
  if (path.extension() != ".h5") {
    return Err("Exchange source is not an HDF5 file");
  }

  H5::H5File file(m_source, H5F_ACC_RDONLY);
  size_t numObjects = static_cast<size_t>(file.getNumObjs());
  for (size_t i = 0; i < numObjects; i++) {
    try {
      String asset_id = file.getObjnameByIdx(i);
      H5::DataSet dataset = file.openDataSet(asset_id + "/data");
      H5::DataSpace dataspace = dataset.getSpace();
      H5::DataSet datasetIndex = file.openDataSet(asset_id + "/datetime");
      H5::DataSpace dataspaceIndex = datasetIndex.getSpace();
      Asset asset(asset_id, m_impl->assets.size());
      m_impl->assets.push_back(std::move(asset));
      auto &asset_ref = m_impl->assets.back();
      loadH5(asset_ref, dataset, dataspace, datasetIndex, dataspaceIndex);
    } catch (H5::Exception &e) {
      return Err<AtlasException>("Error loading asset: " +
                                 std::string(e.getCDetailMsg()));
    } catch (const std::exception &e) {
      return Err<AtlasException>("Error loading asset: " +
                                 std::string(e.what()));
    } catch (...) {
      return Err<AtlasException>("Error loading asset: Unknown error");
    }
  }
#else
  return Err<AtlasException>("HDF5 support is not enabled");
#endif

  return true;
}

END_ATLAS_NAMESPACE