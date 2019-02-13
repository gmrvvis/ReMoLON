set(CPACK_PACKAGE_CONTACT "Juan Pedro Brito Méndez <juanpedro.brito@upm.es>")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_LICENSE "Proprietary")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-test-dev")

include(CommonCPack)
