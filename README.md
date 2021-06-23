# EE2405DEMO
Openmv:
  apriltag以及line detection的code打在一起，並且用uart去寫rpc給mbed 
  line detection:若偵測到線段並且符合過濾器就會輸出線段角度，方法是透過uart形式為rpc function
  apriltag:偵測到apriltag會將tx,tz寫入rpc function並且用uart傳給mbed
mbed:
  讀取uart，將openmv傳來的rpc給處理
  
  linedetection:
    將從openmv得到的角度傳給裡面一個function去做判定以及車子行進修正處理，如果絕對值大於某個值則會讓車子進行為微微轉彎修正(方向則依照角度正負值決定)。並且寫Xbee說明
  偵測線段

  object detection:
    為同時保持ping讀取車子與障礙物的距離，開啟一個thread，將ping的function放入eventqueuey在用thread.start去啟動，其中當距離小於一定值時候便可以透過
  Xbee傳到電腦PC端 說明偵測到障礙物(此非rpc)
  
  apriltag detection:將tx tz傳到xbee，且說明偵測到apritag
