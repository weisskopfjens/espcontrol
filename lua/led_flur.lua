if startup==nil then
 initLED(14,12,13)
 initIR(4,5)
 autooff=60000
 ntpBegin("de.pool.ntp.org",1,0)
 startup=1
 irdelay=0
 setLEDColor(10,255,255)
 attachCounter(0,0,1,0)
 automode=1
 starttime=0
end
daymin=minOfDay()
sunr=(sunrise(49.6020, 10.5213, 1)+45)
suns=(sunset(49.6020, 10.5213, 1)-45)
-- AUTOMODE
if automode == 1 then
if getCounter(0) > 0 and (daymin>suns or daymin<sunr) then
 LEDon()
 udpSend("192.168.178.204","nightlighton=1",28000)
 resetCounter(0)
 starttime=millis()
end
if millis()>starttime+autooff and starttime~=-1 then
 -- LEDoff()
 setAnimationSpeed(170)
 setLEDMode(6)
 starttime=-1
end
end
-- IRRX
ir=getIRValue()
if ir>0 then
 lastir=ir
 if ir==1 and irdelay==0 then
  irdelay=1
  toggleLED()
 end
 if ir==0xE then
  changeLEDcolor(-2,0,0)
 end
 if ir==0x10 then
  changeLEDcolor(2,0,0)
 end
 if ir==0x11 then
  changeLEDcolor(0,0,-2)
 end
 if ir==0xD then
  changeLEDcolor(0,0,2)
 end
 if ir>=0x3 and ir<=0x9 then
  setLEDMode(ir-3)
 end
 if ir==35 then
  changeLEDcolor(0,-2,0)
 end
 if ir==36 then
  changeLEDcolor(0,2,0)
 end
 if ir==37 then
  setAnimationSpeed(getAnimationSpeed()+5)
 end
 if ir==38 then
  setAnimationSpeed(getAnimationSpeed()-5)
 end
end
-- IRDELAY
if irdelay>=1 then
 irdelay=irdelay+1
end
if irdelay>=10 then
 irdelay=0
end
