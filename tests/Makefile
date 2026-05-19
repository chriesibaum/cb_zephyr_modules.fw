TWISTER=../../upstream/zephyr/scripts/twister

MODULE_ROOT=../
NUCLEO_PLATFORM=nucleo_u385rg_q
HW_MAP?=./hardware-map.yml

TWISTER_OUT_GETTING_STARTED=./twister-out__getting_started
TWISTER_OUT_HW=./twister-out__hw
TWISTER_OUT_NATIVE_SIM_COV=./twister-out__native_sim_cov


.phony: list-tests list-tests-hw twister_getting_started twister_hw twister_native_sim_cov clean

list-tests:
	@echo "Listing all available tests in the current directory using Twister..."
	$(TWISTER) -T  ./  --list-tests \
		--device-testing --hardware-map $(HW_MAP)

list-tests-hw:
	@echo "Listing HW tests..."
	$(TWISTER) -T ./ --list-tests -p $(NUCLEO_PLATFORM) \
		--device-testing --hardware-map $(HW_MAP)

all: twister_getting_started twister_hw twister_native_sim_cov

twister_getting_started:
	$(TWISTER) -T  ./cb_getting_started \
		--outdir $(TWISTER_OUT_GETTING_STARTED) \

	junit2html $(TWISTER_OUT_GETTING_STARTED)/twister_report.xml


twister_hw:
	$(TWISTER) -T  ./ \
		--device-testing --hardware-map $(HW_MAP) \
		--outdir $(TWISTER_OUT_HW)

	junit2html $(TWISTER_OUT_HW)/twister_report.xml


twister_native_sim_cov::
	$(TWISTER) -T ./ -p native_sim \
		--coverage-basedir $(MODULE_ROOT) \
		--enable-coverage --coverage --coverage-tool gcovr \
		--coverage-formats html,xml --outdir $(TWISTER_OUT_NATIVE_SIM_COV)

	junit2html $(TWISTER_OUT_NATIVE_SIM_COV)/twister_report.xml


clean:
	@echo "Cleaning build artifacts and test output directories..."
	rm -rf ./twister-out*