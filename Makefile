# Build options
SKETCHES :=        TinyMain TinyDispHigh TinyDispMid TinyDispLow TinyProgSd \
                   UnoGfx UnoWatcher
ifdef UNO
	BOARD :=       arduino:avr:uno
else
	BOARD :=       attiny:avr:ATtinyX5:cpu=attiny85,clock=internal16
endif
BOARD_NAME =       $(subst :,.,$(BOARD))
ifdef OTHER
PORT :=            /dev/ttyACM1
else
PORT :=            /dev/ttyACM0
endif
ASC :=             arduino
SHARED_FILES :=    $(wildcard shared/*)

# Autogenerated project settings
define make-project-target
.PHONY : $1
$1 : $1/$1.ino
	cp shared/* $1/
	$(ASC) --verify --board $(BOARD) $1/$1.ino
	$(foreach file,$(SHARED_FILES),rm $1/$(notdir $(file);))

$1-upload : $1/$1.ino
	cp shared/* $1/
	$(ASC) --upload --board $(BOARD) --port $(PORT) $1/$1.ino
	$(foreach file,$(SHARED_FILES),rm $1/$(notdir $(file);))
endef

# Targets
ALL_BUILDS :=      $(foreach sketch,$(SKETCHES),$(sketch))
.PHONY : all
all : $(ALL_BUILDS)
	

$(foreach sketch,$(SKETCHES), $(eval $(call make-project-target,$(sketch))))

.PHONY : clean
clean :
	$(foreach sketch,$(SKETCHES),\
		$(foreach file,$(SHARED_FILES),\
			rm -rf $(sketch)/$(notdir $(file));\
		)\
	)

.PHONY : list
list :
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$'
