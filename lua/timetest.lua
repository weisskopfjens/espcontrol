if startup==nil then
  initSSD1306(4,5)
  timestr=""
end
timestr=time()
ssd1306clear()
ssd1306drawString(0,0,timestr)
ssd1306drawString(0,20,"test")
ssd1306drawString(0,40,tostring(sunset()/60))
ssd1306display()
