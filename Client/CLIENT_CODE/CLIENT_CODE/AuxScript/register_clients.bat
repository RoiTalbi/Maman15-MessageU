del roi.info

copy /Y ..\..\Debug\CLIENT_CODE.exe .
del me.info

CLIENT_CODE.exe < test1.inp
rename me.info roi.info

CLIENT_CODE.exe < test2.inp
del me.info

CLIENT_CODE.exe < test3.inp
rename me.info meir.info

CLIENT_CODE.exe < test4.inp

CLIENT_CODE.exe < sendMsg.inp

del me.info
rename roi.info me.info

CLIENT_CODE.exe < ReadMessages.inp
::move /Y me.info ..


@pause
