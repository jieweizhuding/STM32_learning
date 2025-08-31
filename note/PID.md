PID是比例、积分、微分的缩写。是一种负反馈调节的实现。

#### 形式介绍

$$
error(t)=target(t)-currunt(t)
$$

PID的连续形式为：
$$
out(t)=K_perror(t)+K_i\int_0^t error(k)dk+K_d \frac{derror(t)}{dt}
$$
离散形式为：
$$
out(t)=K_perror(t)+K_i'\sum_0^terror(k)+K_d'(error(t)-error(t-1))
$$
其中$K_i'=K_i*T$,$K_d'=\frac{K_d}{T}$，T为每两次调节之间的时间差。

其中离散形式又称为位置式PID，如果将两个out做差，我们能够得到增量式PID
$$
\Delta out(k)=K_P(error(k)-error(k-1))\\+K_i'error(k)\\+K_d'(error(k)-2error(k-1)+error(k-2))
$$

#### 功能介绍
接下来分别介绍一下PID三个部分的功能：
1. P（比例）

        类似于模电中的负反馈调节电路，输出一个令目标值反相移动的量。但如果只有P部分，则会导致稳态误差的出现。$K_p$过大，则会导致超调，但太小会导致系统响应太慢。同时$K_p$越大，稳态误差越小。
2. I（积分）
        用于消除稳态误差。若$K_i$越大，则稳态误差消除越快，但同时越大，会导致系统滞后性越大。如要求系统反向，则积分项需要消除前面部分的累积。
3. D（微分）
        用于减小超调的影响，有些时候PI就可以完成负反馈，但有时系统中超调现象难以消除，则需要D。$K_d$越大，系统阻尼越大，但系统也会更加卡顿。


#### 稳态误差
当只有P存在时，设定好目标值后，实际值向目标值靠拢，且某一时刻实际值等于目标值。此时out为0。若out为0时系统会自发变化，即存在“阻力”，会导致out重新从0向目标值靠拢，重复最开始的变化。但与最开始不同的是一方面此时实际值是减小的，另一方面最开始时error很大，并不断减小，而此时error接近于0，因为阻力而有远离0的趋势，但因为out的调节，error有靠近0的趋势，并且最初靠近0的趋势会小于远离0的趋势，而随着实际值的变化使得error靠近0的趋势增大，直到error靠近0的趋势与远离0的趋势相同时，error不再变化，并维持在这个状态。可以认为后半段是实际值减小，用于产生对抗“阻力”的作用，当成功对抗“阻力”后，实际值也就不再变化。
或者我们从Out的变化过程考虑，最初Error较大，Out是一个较大的值，但Error随之减小，Out的值有减小的趋势，Error就有增大的趋势，即Out又有增大的趋势，两者叠加，即Out会收敛到一个定值。即Out先跳变到一个较大的值，再逐渐减小到一个定值。而这个较大的值由Kp决定，所以Kp越大，稳态误差越小。


## 代码实现
要代码实现PID，需要规定周期T，一般通过定时器确定：
```
//全局变量：
int16_t Target,Actual,Out;
float Kp,Ki,Kd;
float Error0,Error1,ErrorInt;


//定时器内部为：
...

    //1.获取Actual，一般通过编码器等实现
    //2.计算Out
    error1=error0;
    error0=Target-Actual;
    errorInt+=error0;

    Out=Kp*error0+Ki*errorInt+Kd*(error0-error1);
    //3.将Out输出到系统中，一般通过PWM实现
...

//使用增量式PID的思路类似，只需调整Out的计算公式，并明确计算得出的为$\Delta Out$即可，真正需要的Out要累加得出。

```

## PID的积分优化

我们观察Out发现，最初Out由Kp决定，跳变增大，并拥有逐渐减小的趋势，而Ki提供的力不仅克服了这个减小的趋势，同时克服了阻力作用，使得Out克服减小趋势，并逐渐增大，当error为0时，Ki仍发挥作用，Out不为0，而是持续输出一个力来对抗阻力。
这在速度控制中是正确的，但在位置控制中就不是了。
位置控制中不存在阻力，故Ki可以为0，但若Ki不为0则会导致超调，所以此时Ki可以为0，也必须为0。但Ki为0会导致当Target与Actual有很小的偏差时，Out很小，可能无法抗衡静摩擦，所以这个很小的误差无法消除。同理，当手动移动一个很小的位置时，也会产生一个很小的误差的效果，这种误差都无法消除。所以有时我们需要消除这样的误差，就需要积分分离。

#### 积分分离
由上述的原因可知，我们需要积分分离来解决小误差的影响，而不要大误差的影响。所以积分分离的思路为，当Error大于某个阈值时，ErrorInt不累计，当Error小于这个阈值时，即Target与Actual很接近时才累计ErrorInt。
```
...
    error1=error0;
    error0=Target-Actual;
    if(fabs(error0)<阈值){
        errorInt+=error0;
    }else{
        errorInt=0;
    }

    Out=Kp*error0+Ki*errorInt+Kd*(error0-error1);
...
```
综上：积分分离可以解决位置调控中Ki不为0导致的超调与小误差。

#### 变速积分
积分分离中最重要的部分在于设定阈值，而若我们提供一个力正好固定误差处于阈值外的一点，Kp作用固定且很微弱，则这个小误差无法被消除。所以我们希望不要直接使用阈值分割，而是让误差越大，积分项的作用越小，误差越大，积分项的作用越大。这个函数需要自己定义，可以使用分段函数、反比例函数等等。
```
...
    error1=error0;
    error0=Target-Actual;
    Inf=1/(k*fabs(error0)+1);
    errorInt+=error0;
    
    Out=Kp*error0+Inf*Ki*errorInt+Kd*(error0-error1);
...
```
需要注意的是，虽然理论上变速积分优于积分分离，但有时项目中积分分离的作用更好，需要取舍。

#### 积分限幅
当系统无论因为卡住或其他原因导致error无法减小时，errorInt会持续增大，导致深度饱和。即当系统恢复后ErrorInt很大，要跟随到Target需要全速抵消errorInt的作用，即系统会较长时间全速运转。我们不希望系统产生这样的情况，所以可以规定errorInt的上限，使得即使出现无法预知的情况，也不要出现深度饱和现象。
```
...
    error1=error0;
    error0=Target-Actual;
    Inf=1/(k*fabs(error0)+1);
    errorInt+=error0;
    if(errorInt>M){
        errorInt=M;
    } else if(errorInt<-M){
        errorInt=-M;
    }
    Out=Kp*error0+Inf*Ki*errorInt+Kd*(error0-error1);
...
```
其中M是上限，注意到积分限幅时error较大时出现的，而积分分离可以屏蔽这种影响，所以理论上确实可以使用积分分离顺便达成积分限幅的效果，但有时这个效果并不是很好，所以还是需要两者共同使用。


## PID的微分优化
#### 微分先行
因为PID在系统刚刚更改时，产生一个巨大的error，即D项产生一个先正向很大的值，后产生一个反向很大的值，然后再产生反向较小的值作为阻尼。我们实际需要的是较小的那些阻尼，而不要前面的两个突变，所以可以定义D项为

$$
D=-(Actual0-Actual1)
$$
这样修改能够消除两个突变，且当P项为0时，D项不会对系统产生影响，使得D项完全成为了阻尼，但实际上会导致系统响应进一步减慢，所以需要取舍：希望输出更平滑则使用微分先行，希望反应更迅速不使用微分先行。
```
...
    Actual1=Actual0;
    Actual0=获取实际值（）;
    

    error1=error0;
    error0=Target-Actual;
    errorInt+=error0;

    Dif=-(Actual0-Actual1);    
    Out=Kp*error0+Ki*errorInt+Kd*Dif;
...
```

#### 不完全微分
不完全微分是解决噪声对D项的影响的。噪声对P、I影响很小，但对D项影响很大，有时噪声影响很严重时，需要考虑使用不完全微分消除噪声的影响。不完全微分就是添加一个低通滤波器。低通滤波器的表达式为：
$$
Y(k)=(1-a)X(k)+aY(k-1)
$$
即本次的值不仅由本次计算值决定，还由上次的值决定。a的值自己定义。
但低通滤波器本身就会导致信号延后，使得系统反应进一步减慢。
所以要减弱噪声影响就使用不完全微分，要系统响应更快就不用。
```
...
    Dif1=Dif0;
    error1=error0;
    error0=Target-Actual;
    errorInt+=error0;
  
     Dif0=(1-a)(error0-error1)+aDif1;
    Out=Kp*error0+Ki*errorInt+Kd*Dif0;
...
```