copy /Y ..\..\Debug\CLIENT_CODE.exe .
del me.info

CLIENT_CODE.exe < test1.inp
del me.info

CLIENT_CODE.exe < test2.inp
del me.info

CLIENT_CODE.exe < test3.inp
del me.info

CLIENT_CODE.exe < test4.inp

move /Y me.info ..
