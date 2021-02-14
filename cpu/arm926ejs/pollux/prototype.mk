
PROTOOBJS = 	prototype/module/mes_adc.o	\
		prototype/module/mes_alive.o	\
		prototype/module/mes_audio.o	\
		prototype/module/mes_clkpwr.o	\
		prototype/module/mes_csc.o	\
		prototype/module/mes_dma.o	\
		prototype/module/mes_dpc.o	\
		prototype/module/mes_ecid.o	\
		prototype/module/mes_gpio.o	\
		prototype/module/mes_i2c.o	\
		prototype/module/mes_intc.o	\
		prototype/module/mes_mcud.o	\
		prototype/module/mes_mcus.o	\
		prototype/module/mes_mlc.o	\
		prototype/module/mes_pwm.o	\
		prototype/module/mes_rtc.o	\
		prototype/module/mes_sdhc.o	\
		prototype/module/mes_sspspi.o	\
		prototype/module/mes_timer.o	\
		prototype/module/mes_uart.o

PROTOSRCS	:= $(PROTOOBJS:.o=.c)
PROTOOBJS	:= $(addprefix $(obj),$(PROTOOBJS))

$(PROTOLIB):	$(PROTOOBJS)
	$(AR) $(ARFLAGS) $@ $(PROTOOBJS)

