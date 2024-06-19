#pragma once
#include "standard/AtlasTypes.hpp"
#include <Eigen/Dense>

#include "Asset.hpp"

BEGIN_ATLAS_NAMESPACE

struct ExchangeImpl {
  String name;
  String source;
  size_t id = 0;
  Vector<Asset> assets;
  Vector<Int64> timestamps;
  HashMap<String, size_t> asset_id_map;
  HashMap<String, size_t> headers;
  Eigen::MatrixXd data;
  Eigen::MatrixXd returns;
  size_t col_count = 0;
  size_t close_index = 0;
  size_t current_index = 0;
  size_t exchange_offset = 0;

  ExchangeImpl(String _name, String _source, size_t id) noexcept
      : name(_name), source(_source), id(id) {}
};

END_ATLAS_NAMESPACE