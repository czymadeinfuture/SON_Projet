from PyQt5.QtWidgets import QApplication, QMainWindow, QGraphicsView, QGraphicsScene, QGraphicsPixmapItem, QPushButton, QLabel, QSlider, QDial, QVBoxLayout, QHBoxLayout, QWidget
from PyQt5.QtCore import QObject, QPropertyAnimation, pyqtProperty, Qt
from PyQt5.QtGui import QPixmap
from sys import argv, exit
import serial
import time

arduino = serial.Serial('COM6', 9600)  # connect to Arduino's port
time.sleep(2) # wait for connection

# creation of the record
class Cp_Comp_1(QObject):
    def __init__(self):
        super(Cp_Comp_1, self).__init__()
        # load picture
        pixmap_1 = QPixmap('record.png')
        # set size of the pic
        height = 300
        width = 300
        scaledPixmap_1 = pixmap_1.scaled(width,height)
        
        self.animation()
        # define QGraphicsPixmapItem
        self.pixmap_item_1 = QGraphicsPixmapItem(scaledPixmap_1)
        # set the center of the rotation
        self.pixmap_item_1.setTransformOriginPoint(0.5*width,0.5*height)   # 中心
        # init position
        self.pixmap_item_1.setPos(0,30)

    # realize the rotation
    def set_rotation(self,degree):
        self.pixmap_item_1.setRotation(degree)
            
    def animation(self):
        self.anim = QPropertyAnimation(self, b'rotation')   # type of animation
        self.anim.setDuration(20000)     # speed of the rotation 
        self.anim.setStartValue(0)  # init angle
        self.anim.setEndValue(360)  # end angle
        self.anim.setLoopCount(-1)  # number of loop 
    rotation = pyqtProperty(int, fset=set_rotation)    # pass the signal

# creation of the needle
class Cp_Comp_2(QObject):
    def __init__(self):
        super(Cp_Comp_2, self).__init__()
        # load picture
        pixmap_2 = QPixmap('needle.png')
        # set the size of pic
        scaledPixmap_2 = pixmap_2.scaled(100,100)
        # define QGraphicsPixmapItem
        self.pixmap_item_2 = QGraphicsPixmapItem(scaledPixmap_2)
        # set the center of rotation
        self.pixmap_item_2.setTransformOriginPoint(80,5) 
        # init position
        self.pixmap_item_2.setPos(70,12)
        # init angle
        self.pixmap_item_2.setRotation(45)
        
        self.animation() 

    def set_rotation(self,degree):
        self.pixmap_item_2.setRotation(degree)    
    def animation(self):
        # move of start
        self.anim_1 = QPropertyAnimation(self, b'rotation')
        self.anim_1.setStartValue(45) 
        self.anim_1.setEndValue(10)  
        self.anim_1.setLoopCount(1)  
        # move of end
        self.anim_2 = QPropertyAnimation(self, b'rotation')
        self.anim_2.setStartValue(10)  
        self.anim_2.setEndValue(45)  
        self.anim_2.setLoopCount(1)  

    rotation = pyqtProperty(int, fset=set_rotation)    

# display the interface
class VinylDisplay(QGraphicsView):
    def __init__(self, parent=None):
        super(VinylDisplay, self).__init__(parent)
        self.initView()

    def initView(self):
        self.cp_comp_1 = Cp_Comp_1()
        self.cp_comp_2 = Cp_Comp_2()
        self.isPlaying = False  
        self.isStopped = True  
        self.scene = QGraphicsScene(self)
        self.scene.setSceneRect(0,0,200,200)    

        self.scene.addItem(self.cp_comp_1.pixmap_item_1)   
        self.scene.addItem(self.cp_comp_2.pixmap_item_2)   

        # set the position of the record and needle
        self.updateItemsPosition()
        self.setScene(self.scene)       
        
        a = self.scene.width()
        btn_1 = QPushButton('PLAY',self)
        btn_1.setGeometry(int(a/3)-10,330,40,40)
        btn_1.clicked.connect(self.start)
        btn_2 = QPushButton('STOP',self)
        btn_2.setGeometry(int(a/3)+155,330, 40, 40)
        btn_2.clicked.connect(self.stop)
        btn_exit = QPushButton('EXIT', self)
        btn_exit.setGeometry(int(a/3)+315,330, 40, 40)
        btn_exit.clicked.connect(self.exit_app)

    def updateItemsPosition(self):
        # calculate the proper position
        recordCenterX = (self.scene.width() - self.cp_comp_1.pixmap_item_1.pixmap().width()) / 2
        recordCenterY = (self.scene.height() - self.cp_comp_1.pixmap_item_1.pixmap().height()) / 2
        self.cp_comp_1.pixmap_item_1.setPos(recordCenterX, recordCenterY)

        needleCenterX = recordCenterX + (self.cp_comp_1.pixmap_item_1.pixmap().width() / 2) +(self.cp_comp_2.pixmap_item_2.pixmap().width() / 2) 
        needleCenterY = recordCenterY - (self.cp_comp_2.pixmap_item_2.pixmap().height() / 2) + 30  # 根据需要调整偏移量
        self.cp_comp_2.pixmap_item_2.setPos(needleCenterX, needleCenterY)

    '''def resizeEvent(self, event):
        self.scene.setSceneRect(0,0,200,200)
        self.updateItemsPosition()
        super(VinylDisplay, self).resizeEvent(event)'''

    def exit_app(self):
        QApplication.instance().quit()

    def start(self):
        if not self.isPlaying: 
            self.cp_comp_1.anim.start()
            self.cp_comp_2.anim_1.start()
            self.isPlaying = True  
            self.isStopped = False  
            arduino.write(b'P')

    def stop(self):
        if self.isPlaying and not self.isStopped:  
            self.cp_comp_1.anim.pause()
            self.cp_comp_2.anim_2.start()
            self.isStopped = True
            self.isPlaying = False
            arduino.write(b'S')

class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setWindowTitle("Vinyl Simulator")
        ##self.setAttribute(Qt.WA_TranslucentBackground)  # transparent bg
        self.setWindowFlags(Qt.FramelessWindowHint) 
        self.setStyleSheet('background:white')
        self.resize(1000, 400)

        self.centralWidget = QWidget()
        self.setCentralWidget(self.centralWidget)
        self.mainLayout = QHBoxLayout(self.centralWidget)

        self.vinylDisplay = VinylDisplay()
        self.mainLayout.addWidget(self.vinylDisplay)

        self.controlsWidget = self.initControls()
        self.mainLayout.addWidget(self.controlsWidget)

    def initControls(self):
        # container 
        controlsWidget = QWidget()
        controlsLayout = QHBoxLayout(controlsWidget)

        # wow slider 
        self.wowSlider = QSlider(Qt.Vertical)
        self.wowSlider.setMinimum(0)
        self.wowSlider.setMaximum(5)  # range
        self.wowSlider.setValue(0)  # default
        self.wowLabel = QLabel('Wow: 0%')
        self.wowLabel.setFixedSize(100, 30)
        self.wowSlider.valueChanged.connect(lambda: self.wowLabel.setText(f'Wow: {self.wowSlider.value()}%'))

        # hiss slider
        self.hissSlider = QSlider(Qt.Vertical)
        self.hissSlider.setMinimum(0)
        self.hissSlider.setMaximum(10)  
        self.hissSlider.setValue(0)  
        self.hissLabel = QLabel('Hiss: 0dB')
        self.hissLabel.setFixedSize(100, 30)
        self.hissSlider.valueChanged.connect(lambda: self.hissLabel.setText(f'Hiss: {self.hissSlider.value()}dB'))

        # 创建gain滑块
        self.gainSlider = QSlider(Qt.Vertical)
        self.gainSlider.setMinimum(0)
        self.gainSlider.setMaximum(100)  
        self.gainSlider.setValue(50)  
        self.gainLabel = QLabel(f'Gain: {self.gainSlider.value()}')
        self.gainLabel.setFixedSize(100, 30)
        self.gainSlider.valueChanged.connect(lambda: (self.changeGain(self.gainSlider.value()), self.gainLabel.setText(f'Gain: {self.gainSlider.value()}')))

        for i, (label, slider) in enumerate([(self.wowLabel, self.wowSlider), (self.hissLabel, self.hissSlider), (self.gainLabel, self.gainSlider)]):
            vbox = QVBoxLayout()
            vbox.addWidget(label)
            vbox.addWidget(slider)
            controlsLayout.addLayout(vbox)
            if i < 2:  # add distance for modifying the layout
                controlsLayout.addSpacing(80) 
        
        return controlsWidget
    
    def changeGain(self,gain):
        arduino.write(b'G')
        arduino.write(bytes([gain]))


app = QApplication(argv)
window = MainWindow()
window.show()
exit(app.exec_())
