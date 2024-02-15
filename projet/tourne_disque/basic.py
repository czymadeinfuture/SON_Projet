from PyQt5.QtWidgets import QApplication, QGraphicsView, QGraphicsScene, QGraphicsPixmapItem, QPushButton, QLabel, QSlider, QDial, QVBoxLayout, QHBoxLayout, QWidget
from PyQt5.QtCore import QObject, QPropertyAnimation, pyqtProperty, Qt
from PyQt5.QtGui import QPixmap
from sys import argv, exit

class Cp_Comp_1(QObject):
    def __init__(self):
        super(Cp_Comp_1, self).__init__()
        # 加载图片资源
        pixmap_1 = QPixmap('record.png')
        # 按比例设置图片大小
        scaledPixmap_1 = pixmap_1.scaled(150,150)
        # 初始化动作
        self.animation()
        # 定义QGraphicsPixmapItem
        self.pixmap_item_1 = QGraphicsPixmapItem(scaledPixmap_1)
        # 设置item旋转的中心点
        self.pixmap_item_1.setTransformOriginPoint(75,75)   # 中心
        # 设置图片的初始位置
        self.pixmap_item_1.setPos(0,30)

    def _set_rotation(self,degree):
        self.pixmap_item_1.setRotation(degree)    # 自身改变角度
    def animation(self):
        # 创建唱片360°无死角转动
        self.anim = QPropertyAnimation(self, b'rotation')   # 动画类型('rotation':转动,'pos':位置移动)
        self.anim.setDuration(20000)     # 运行的秒速(经测试，网易云用户每播放40s的歌曲，世界上就会有一张无辜唱片被转动一周)
        self.anim.setStartValue(0)  # 起始角度
        self.anim.setEndValue(360)  # 结束角度
        self.anim.setLoopCount(-1)  # 设置循环次数
    rotation = pyqtProperty(int, fset=_set_rotation)    # 属性动画改变自身数值(传递信号？？)

"""把柄"""
class Cp_Comp_2(QObject):
    def __init__(self):
        super(Cp_Comp_2, self).__init__()
        # 加载图片资源
        pixmap_2 = QPixmap('needle.png')
        # 按比例设置图片大小
        scaledPixmap_2 = pixmap_2.scaled(85,85)
        # 定义QGraphicsPixmapItem
        self.pixmap_item_2 = QGraphicsPixmapItem(scaledPixmap_2)
        # 设置item旋转的中心点
        self.pixmap_item_2.setTransformOriginPoint(80,5) 
        # 设置图片的初始位置
        self.pixmap_item_2.setPos(70,12)
        self.pixmap_item_2.setRotation(45)
        # 初始化动作
        self.animation()
        

    def set_rotation(self,degree):
        self.pixmap_item_2.setRotation(degree)    # 自身改变角度
    def animation(self):
        # 转轴转动的动画之开和关
        self.anim_1 = QPropertyAnimation(self, b'rotation')
        self.anim_1.setStartValue(45)  # 起始角度
        self.anim_1.setEndValue(20)  # 结束角度
        self.anim_1.setLoopCount(1)  # 设置循环次数
        self.anim_2 = QPropertyAnimation(self, b'rotation')
        self.anim_2.setStartValue(20)  # 起始角度
        self.anim_2.setEndValue(45)  # 结束角度
        self.anim_2.setLoopCount(1)  # 设置循环次数

    rotation = pyqtProperty(int, fset=set_rotation)    # 属性动画改变自身数值(传递信号？？)
"""呈现动画的界面"""
class View(QGraphicsView):
    def __init__(self):
        super(View, self).__init__()
        self.setWindowTitle("Vinyl Simulator")
        self.resize(800,300)
        ##self.setAttribute(Qt.WA_TranslucentBackground)  # 透明背景
        self.setWindowFlags(Qt.FramelessWindowHint) # 无边框
        self.setStyleSheet('background:white')
        self.initView()

    def initView(self):
        self.cp_comp_1 = Cp_Comp_1()
        self.cp_comp_2 = Cp_Comp_2()
        self.scene = QGraphicsScene(self)
        self.scene.setSceneRect(-10,0,170,210)    # 设置动画区域，在此区域内的控件会被动态刷新
        self.scene.addItem(self.cp_comp_1.pixmap_item_1)   # 往容器里面添加item
        self.scene.addItem(self.cp_comp_2.pixmap_item_2)   # 往容器里面添加item
        self.setScene(self.scene)       # 英文翻译:设置场景？？

        btn_1 = QPushButton('PLAY',self)
        btn_1.setGeometry(10,178,40,40)
        btn_1.clicked.connect(self.start)
        btn_2 = QPushButton('STOP',self)
        btn_2.setGeometry(74, 178, 40, 40)
        btn_2.clicked.connect(self.stop)
        btn_exit = QPushButton('EXIT', self)
        btn_exit.setGeometry(138, 178, 40, 40)
        btn_exit.clicked.connect(self.exit_app) 

    def exit_app(self):
        QApplication.instance().quit()

    def start(self):
        self.cp_comp_1.anim.start()
        self.cp_comp_2.anim_1.start()
    def stop(self):
        self.cp_comp_1.anim.pause()
        self.cp_comp_2.anim_2.start()

app = QApplication(argv)
vi = View()
vi.show()
exit(app.exec_())