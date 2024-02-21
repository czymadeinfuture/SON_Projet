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
        btn_2.setGeometry(int(a/3)+98,330, 40, 40)
        btn_2.clicked.connect(self.stop)
        btn_3 = QPushButton('NEXT',self)
        btn_3.setGeometry(int(a/3)+206,330, 40, 40)
        btn_3.clicked.connect(self.next)
        btn_exit = QPushButton('EXIT', self)
        btn_exit.setGeometry(int(a/3)+315,330, 40, 40)
        btn_exit.clicked.connect(self.exit_app)

    def updateItemsPosition(self):
        # calculate the proper position
        recordCenterX = (self.scene.width() - self.cp_comp_1.pixmap_item_1.pixmap().width()) / 2
        recordCenterY = (self.scene.height() - self.cp_comp_1.pixmap_item_1.pixmap().height()) / 2
        self.cp_comp_1.pixmap_item_1.setPos(recordCenterX, recordCenterY)

        needleCenterX = recordCenterX + (self.cp_comp_1.pixmap_item_1.pixmap().width() / 2) +(self.cp_comp_2.pixmap_item_2.pixmap().width() / 2) 
        needleCenterY = recordCenterY - (self.cp_comp_2.pixmap_item_2.pixmap().height() / 2) + 30  
        self.cp_comp_2.pixmap_item_2.setPos(needleCenterX, needleCenterY)

    def resizeEvent(self, event):
        self.scene.setSceneRect(0,0,200,200)
        self.updateItemsPosition()
        super(VinylDisplay, self).resizeEvent(event)

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

    def next(self):
        if not self.isPlaying: 
            self.cp_comp_1.anim.start()
            self.cp_comp_2.anim_1.start()
            self.isPlaying = True  
            self.isStopped = False  
            arduino.write(b'X')

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
        self.wowSlider.setMaximum(8)  # range
        self.wowSlider.setValue(4)  # default
        self.wowLabel = QLabel('Wow: 4%')
        self.wowLabel.setFixedSize(100, 30)
        self.wowSlider.valueChanged.connect(lambda: (self.changeWow(self.wowSlider.value()),self.wowLabel.setText(f'Wow: {self.wowSlider.value()}%')))

        # noise slider
        self.noiseSlider = QSlider(Qt.Vertical)
        self.noiseSlider.setMinimum(0)
        self.noiseSlider.setMaximum(5)  
        self.noiseSlider.setValue(2)  
        self.noiseLabel = QLabel(f'Noise: {self.noiseSlider.value()}dB')
        self.noiseLabel.setFixedSize(100, 30)
        self.noiseSlider.valueChanged.connect(lambda: (self.changeNoise(self.noiseSlider.value()),self.noiseLabel.setText(f'Noise: {self.noiseSlider.value()}dB')))

        # gain slider
        self.gainSlider = QSlider(Qt.Vertical)
        self.gainSlider.setMinimum(0)
        self.gainSlider.setMaximum(100)  
        self.gainSlider.setValue(50)  
        self.gainLabel = QLabel(f'Gain: {self.gainSlider.value()}')
        self.gainLabel.setFixedSize(100, 30)
        self.gainSlider.valueChanged.connect(lambda: (self.changeGain(self.gainSlider.value()), self.gainLabel.setText(f'Gain: {self.gainSlider.value()}')))

        for i, (label, slider) in enumerate([(self.wowLabel, self.wowSlider), (self.noiseLabel, self.noiseSlider), (self.gainLabel, self.gainSlider)]):
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

    def changeNoise(self,noise):
        arduino.write(b'N')
        arduino.write(bytes([noise]))

    def changeWow(self,wow):
        arduino.write(b'M')
        arduino.write(bytes([wow]))


app = QApplication(argv)
window = MainWindow()
window.show()
exit(app.exec_())
