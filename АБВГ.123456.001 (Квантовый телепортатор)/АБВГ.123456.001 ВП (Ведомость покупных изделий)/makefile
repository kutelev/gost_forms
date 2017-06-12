SHELL := bash
ROOT_DIR := $(shell BASE='..'; \
              while [[ ! $$BASE =~ Common ]]; do \
                  DIR=`find $$BASE -maxdepth 1 -name Common -type d`; \
                  if [[ ! $$DIR =~ Common ]]; then \
                      BASE="../"$$BASE; \
                  else \
                      break; \
                  fi \
              done; \
              echo $$BASE)

include $(ROOT_DIR)/Common/common.mak
include $(ROOT_DIR)/Common/part.mak
