#pragma once
#include <qmainwindow.h>

#include "atlasx/atlasx_impl.hpp"
#include "atlasx_standard.hpp"

using namespace Atlas;

namespace AtlasX {

struct AtlasXManagerImpl;

class AtlasXManager : public QMainWindow {
  Q_OBJECT
public:
  AtlasXManager(String manager_id, AtlasXImpl &app_impl, QWidget *parent);
  ~AtlasXManager();

  virtual AtlasXManagerType type() const = 0;
  String const &managerId() const noexcept;

  QIcon svgIcon(const QString &File) {
    QIcon SvgIcon(File);
    SvgIcon.addPixmap(SvgIcon.pixmap(50));
    return SvgIcon;
  }

protected:
  AtlasXImpl &appImpl() noexcept;

private:
  UniquePtr<AtlasXManagerImpl> m_impl;
};

} // namespace AtlasX