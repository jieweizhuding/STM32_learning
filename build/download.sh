# TODO 需要更改项目名称
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program Templet.hex verify reset exit"