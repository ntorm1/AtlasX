#pragma once

#include <QtWidgets/QMainWindow>

#include "atlasx_declare.hpp"
#include "standard/AtlasTypes.hpp"

using namespace Atlas;
using namespace AtlasX;

QT_BEGIN_NAMESPACE
namespace Ui {
class AtlasXClass;
};
QT_END_NAMESPACE

struct AtlasXComponents;

namespace AtlasX {

class AtlasXApp : public QMainWindow {
  Q_OBJECT

public:
  AtlasXApp(QWidget *parent = nullptr);
  ~AtlasXApp();

  void initToolbar() noexcept;
  void initBottomToolbar() noexcept;
  void onBuild() noexcept;

  void onNewWorkSpace() noexcept;
  void onSaveWorkSpace() noexcept;
  void onLoadWorkSpace() noexcept;

private:
  Ui::AtlasXClass *ui;
  UniquePtr<AtlasXImpl> impl;
  UniquePtr<AtlasXComponents> components;
};
} // namespace AtlasX