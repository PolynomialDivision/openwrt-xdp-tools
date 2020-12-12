#
# This software is licensed under the Public Domain.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=xdp-tools
PKG_SOURCE_DATE:=2020-12-07
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/xdp-project/xdp-tools.git
PKG_SOURCE_VERSION:=7cad843311bc1d67ab0dc730ae1ef97a7040b9d5
PKG_MIRROR_HASH:=skip

PKG_MAINTAINER:=Nick Hainke <vincent@systemli.org>
PKG_LICENSE:=GPL-2.0-only
PKG_LICENSE_FILES:=LICENSE

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/xdp-tools
  SECTION:=net
  CATEGORY:=Network
  TITLE:=XDP Tools
  URL:=https://github.com/xdp-project/xdp-tools.git
  DEPENDS:=+libbpf +libpcap
endef

define Package/xdp-tools/description
  Load xdp scripts into kernel.
endef

define Package/xdp-tools/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/xdp-loader/xdp-loader $(1)/usr/bin/xdp-loader
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/xdp-dump/xdpdump $(1)/usr/bin/xdpdump
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/xdp-filter/xdp-filter $(1)/usr/bin/xdp-filter
endef

$(eval $(call BuildPackage,xdp-tools))
