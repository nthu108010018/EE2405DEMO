# EE2405DEMO
Openmv:
  使用apriltag，透過讀取到的tag.x_translation以及tag.z_translation相除取arctan得到一個車子與apriltag的角度，透過uart傳給mbed
mbed:
  讀取uart，將從openmv得到的角度傳給裡面一個function去做判定以及車子行進修正處理，如果絕對值大於某個值則會讓車子進行為微微轉彎修正(方向則依照角度正負值決定)。
  為同時保持ping讀取車子與障礙物的距離，開啟一個thread，將ping的function放入eventqueuey在用thread.start去啟動，其中讀取回來的距離以及先前傳過來的角度則會通
  Xbee傳到電腦PC端，透過sudo screen /dev/ttyUSB0 即可查看車子相對於apriltag的位置，座標呈現方式為極座標螢幕每隔數秒便會顯示一個與障礙物距離以及apriltag角度
