# Enable build support for NFC open source vendor modules
NQ_VENDOR_NFC += vendor.nxp.hardware.nfc@2.0-service
NQ_VENDOR_NFC += nfc_nci.nqx.default.hw

ifeq ($(strip $(TARGET_USES_NQ_NFC)),true)
PRODUCT_COPY_FILES += \
    vendor/nxp/opensource/halimpl/SN100x/halimpl/libnfc-nci.conf:$(TARGET_COPY_OUT_VENDOR)/etc/libnfc-nci.conf

PRODUCT_PACKAGES += $(NQ_VENDOR_NFC)
endif

