# _PROCESS_
input -> stereo to mono -> lowpass filter (signal bandwidth) -> Clicks -> Thumps -> Hiss -> Wow & flutter -> lowpass filter -> horn modeling 

## 阶段解释
stereo to mono : 立体声道变成单声道  
lowpass filter ： 低通滤波器  
(signal bandwidth) : 信号带宽。在旧的录音中，存储的音频信号带宽较窄，最低和最高频率被丢失。模拟录音的带宽受到唱片和唱针的大小和速度以及唱片槽的大小和形状的限制。  
                   低频的限制取决于唱针的弹性，最大的高频由唱针在试图移动时与唱片槽的摩擦定义。  
distortion ： 失真，原因包括包括立体声黑胶唱片上的跟踪失真、唱片槽壁的变形、播放器拾音器的非线性行为以及放大器的非线性。无需对每种情况进行模拟，结果都是相同的，谐波失真和波形在大信号水平时的剪切。  
clicks : 点击是时间和幅度随机的脉冲性干扰, 来源可能多种多样,在模拟唱片录音中，最常见的原因是附着在槽中的灰尘和污垢，以及制作唱片时使用的材料中的颗粒状物质。  
thumps : 当唱片或圆柱槽壁严重损坏时，产生的退化效应被称为“低频脉冲”或“跳动声”，跳动声与唱片表面上的深划痕或使用胶水修复破碎唱片部分时产生的接缝有关。  
         唱针臂被这些不连续性激发，并且唱针臂的缓慢波动脉冲响应被叠加到未受损失的音频信号上。这种类型的失真通常可以描述为短暂而强烈的不连续性，类似于持续时间短于2毫秒的点击，随后是低长而衰减的低频内容，通常持续时间超过50毫秒。  
hiss : 嘶嘶声。这种随机的附加背景噪声来自不同的来源：电路噪声、存储介质的不规则性以及录音环境中的环境噪声，但这种退化被认为是单一的噪声过程，这种噪声的随机性质使其存在于所有频率中。  
Wow & flutter : Wow和flutter是在原始表演中并不存在的音高变化缺陷。Wow指的是音高（即速度）的缓慢变化，而flutter指的是更快的波动。这些波动通常是周期性的，它们可能发生在录音或播放过程中。  
                Wow发生的一个可能机制是录音介质旋转速度的变化。另一个原因是在唱片和圆筒音频录音过程中出现的偏心，或者在复制或再现时，错误冲压的留声机唱片。  
horn modeling : 模拟喇叭。在理想情况下，系统对所有频率的处理都应该相同，这样听到的声音才是原始的，但喇叭对某些频率的强调以及对其他频率的衰减使得最终的声音具有特殊的味道。  

## 阶段实现方法
stereo to mono : 最广泛使用的方法是计算两个声道的平均值.  
lowpass filter : 两个butterworth filter  
                
                import("stdfaust.lib");

                // Define Butterworth lowpass filter coefficients
                ButterworthLowpass(fc) = 
                      fi.lowpass(1, 2*fc);

                // Define the Butterworth filter and its output
                process = ButterworthLowpass(1000) : _;

distortion : 模拟这种失真的选择方式是使用两种不同的函数：一种用于在大声部分创建非线性，另一种用于软音频样本。  
             双曲正切函数用于第一个目的，因为它在信号的低值时具有线性特性，并且在高信号值时饱和。通过引入一个参数base并对函数进行归一化，可以控制引入的失真量.  
             y = tanh(base * x)/tanh(base)  
             对于软通道，使用方程式:   
             y = sign(x)⋅ abs(x) **base    
             基数越高，引入的失真就越多.  
clicks : 
