# 【NO.67】面试题:Linux网络编程中可靠UDP，KCP协议快在哪

## 1.前言

云真机已经支持手机端的画面投影。云真机实时操作，对延迟的要求比远程视频对话的要求更高（100ms以内）。在无线网络下，如何更实时、更可靠的传输视频流就成了一个挑战。**通过websocket、RTMP、UDP的比较，最后选择了可靠的UDP协议KCP来进行实时音视频的传输。**

## 2.KCP简介

KCP是一个快速可靠协议，能以比 TCP浪费10%-20%的带宽的代价，换取平均延迟降低 30%-40%，且最大延迟降低三倍的传输效果。纯算法实现，并不负责底层协议（如UDP）的收发，需要使用者自己定义下层数据包的发送方式，以 callback的方式提供给 KCP。 连时钟都需要外部传递进来，内部不会有任何一次系统调用。本文传输协议之考虑UDP的情况。

名词说明（源码字段）：

- 用户数据：应用层发送的数据，如一张图片2Kb的数据。
- MTU：最大传输单元。即每次发送的最大数据。
- RTO：Retransmission TimeOut，重传超时时间。
- cwnd:congestion window，拥塞窗口，表示发送方可发送多少个KCP数据包。与接收方窗口有关，与网络状况（拥塞控制）有关，与发送窗口大小有关。
- rwnd:receiver window,接收方窗口大小，表示接收方还可接收多少个KCP数据包。
- snd_queue:待发送KCP数据包队列。
- snd_nxt:下一个即将发送的kcp数据包序列号。
- snd_una:下一个待确认的序列号。

## 3.KCP使用方式

### 3.1 创建 KCP对象

```text
// 初始化 kcp对象，conv为一个表示会话编号的整数，和tcp的 conv一样，通信双

// 方需保证 conv相同，相互的数据包才能够被认可，user是一个给回调函数的指针

ikcpcb *kcp = ikcp_create(conv, user);
```

### 3.2 设置传输[回调](https://link.zhihu.com/?target=https%3A//so.csdn.net/so/search%3Fq%3D%E5%9B%9E%E8%B0%83%26spm%3D1001.2101.3001.7020)函数（如UDP的send函数）

```text
// KCP的下层协议输出函数，KCP需要发送数据时会调用它

// buf/len 表示缓存和长度

// user指针为 kcp对象创建时传入的值，用于区别多个 KCP对象

int udp_output(const char *buf, int len, ikcpcb *kcp, void *user)

{

  .... 

}

// 设置回调函数

kcp->output = udp_output;
```

### 3.3 循环调用 update

```text
// 以一定频率调用 ikcp_update来更新 kcp状态，并且传入当前时钟（毫秒单位）

// 如 10ms调用一次，或用 ikcp_check确定下次调用 update的时间不必每次调用

ikcp_update(kcp, millisec);
```

### 3.4 输入一个[应用层](https://link.zhihu.com/?target=https%3A//so.csdn.net/so/search%3Fq%3D%E5%BA%94%E7%94%A8%E5%B1%82%26spm%3D1001.2101.3001.7020)数据包（如UDP收到的数据包）

```text
// 收到一个下层数据包（比如UDP包）时需要调用：ikcp_input(kcp,received_udp_packet,received_udp_size);
```

处理了下层协议的输出/输入后 KCP协议就可以正常工作了，使用 ikcp_send 来向远端发送数据。而另一端使用 ikcp_recv(kcp, ptr, size)来接收数据。

![img](https://pic2.zhimg.com/80/v2-5fc605ca1b452033cf61821cb98cbfe5_720w.webp)

总结：UDP收到的包，不断通过kcp_input喂给KCP，KCP会对这部分数据（KCP协议数据）进行解包，重新封装成应用层用户数据，应用层通过kcp_recv获取。应用层通过kcp_send发送数据，KCP会把用户数据拆分kcp数据包，通过kcp_output，以UDP（send）的方式发送。

## 4.KCP使用方式

**这部分KCP文档有介绍，理解KCP协议无需过于关注。协议默认模式是一个标准的 ARQ，需要通过配置打开各项加速开关：**

### 4.1 工作模式

```text
int ikcp_nodelay(ikcpcb *kcp, int nodelay, int interval, int resend, int nc)
```

- nodelay ：是否启用 nodelay模式，0不启用；1启用。
- interval ：协议内部工作的 interval，单位毫秒，比如 10ms或者 20ms。
- resend ：快速重传模式，默认0关闭，可以设置2（2次ACK跨越将会直接重传）。
- nc ：是否关闭流控，默认是0代表不关闭，1代表关闭。

**普通模式： ikcp_nodelay(kcp, 0, 40, 0, 0);**

**极速模式： ikcp_nodelay(kcp, 1, 10, 2, 1);**

### 4.2 最大窗口

```text
int ikcp_wndsize(ikcpcb *kcp, int sndwnd, int rcvwnd);
```

该调用将会设置协议的最大发送窗口和最大接收窗口大小，默认为32. 这个可以理解为 TCP的 SND_BUF 和 RCV_BUF，只不过单位不一样 SND/RCV_BUF 单位是字节，这个单位是包。

### 4.3 最大传输单元

纯算法协议并不负责探测 MTU，默认 mtu是1400字节，可以使用ikcp_setmtu来设置该值。该值将会影响数据包归并及分片时候的最大传输单元。

### 4.4 最小RTO

不管是 TCP还是 KCP计算 RTO时都有最小 RTO的限制，即便计算出来RTO为40ms，由于默认的 RTO是100ms，协议只有在100ms后才能检测到丢包，快速模式下为30ms，可以手动更改该值：
**kcp->rx_minrto = 10;**



## 5.KCP为什么存在

首先要看TCP与UDP的区别，TCP与UDP都是传输层的协议，比较两者的区别主要应该是说TCP比UDP多了什么:

- 面向连接：TCP接收方与发送方维持了一个状态（建立连接，断开连接），双方知道对方还在。
- 可靠的：发送出去的数据对方一定能够接收到，而且是按照发送的顺序收到的。
- 流量控制与拥塞控制：TCP靠谱通过滑动窗口确保，发送的数据接收方来得及收。TCP无私，发生数据包丢失的时候认为整个网络比较堵，自己放慢数据发送速度。

TCP协议的可靠与无私让使用TCP开发更为简单，同时它的这种设计也导致了慢的特点。UDP协议简单，所以它更快。但是，UDP毕竟是不可靠的，应用层收到的数据可能是缺失、乱序的。KCP协议就是在保留UDP快的基础上，提供可靠的传输，应用层使用更加简单。

其他差别，TCP以字节流的形式，UDP以数据包的形式。很多人以为，UDP是不可靠的，所以sendto(1000),接收端recvfrom(1000)可能会收到900。这个是错误的。所谓数据包，就是说UDP是有界的，sendto(300),sendto(500)；接收到，recvfrom(1000),recvfrom(1000)那么可能会收到300，500或者其中一个或者都没收到。UDP应用层发送的数据，在接收缓存足够的情况下，要么收到全的，要么收不到。

总结：TCP可靠简单，但是复杂无私，所以速度慢。KCP尽可能保留UDP快的特点下，保证可靠。

## 6.KCP原理

### 6.1 协议简介

KCP是一个可靠的传输协议，UDP本身是不可靠的，所以需要额外信息来保证传输数据的可靠性。**因此，我们需要在传输的数据上增加一个包头。用于确保数据的可靠、有序。**

![img](https://pic2.zhimg.com/80/v2-fe57c61fd733e26bf1ac9fa419cf2c31_720w.webp)

- conv:连接号。UDP是无连接的，conv用于表示来自于哪个客户端。对连接的一种替代。
- cmd:命令字。如，IKCP_CMD_ACK确认命令，IKCP_CMD_WASK接收窗口大小询问命令，IKCP_CMD_WINS接收窗口大小告知命令。
- frg:分片，用户数据可能会被分成多个KCP包，发送出去。
- wnd:接收窗口大小，发送方的发送窗口不能超过接收方给出的数值
- ts:时间序列。
- sn:序列号。
- una:下一个可接收的序列号。其实就是确认号，收到sn=10的包，una为11。
- len：数据长度。
- data:用户数据。

后面的讲解，主要以极速模式：**ikcp_nodelay(kcp, 1, 10, 2, 1)为主，启用nodelay设置，刷新间隔控制在10ms，开启快速重传模式，关闭流量控制。**

### 6.2 数据发送过程

1.数据发送准备

用户发送数据的函数为ikcp_send。

ikcp_send(ikcpcb kcp, const char buffer, int len)

该函数的功能非常简单，把用户发送的数据根据MSS进行分片。如上图，用户发送1900字节的数据，MTU为1400byte。因此，该函数会把1900byte的用户数据分成两个包，一个数据大小为1400，头frg设置为1，len设置为1400；第二个包，头frg设置为0，len设置为500。切好KCP包之后，放入到名为snd_queue的待发送队列中。

注：流模式情况下，kcp会把两次发送的数据衔接为一个完整的kcp包。非流模式下，用户数据%MSS的包，也会作为一个包发送出去。

MTU，数据链路层规定的每一帧的最大长度，超过这个长度数据会被分片。通常MTU的长度为1500字节，IP协议规定所有的路由器均应该能够转发（512数据+60IP首部+4预留=576字节）的数据。MSS，最大输出大小（双方的约定），KCP的大小为MTU-kcp头24字节。IP数据报越短，路由器转发越快，但是资源利用率越低。传输链路上的所有MTU都一至的情况下效率最高，应该尽可能的避免数据传输的工程中，再次被分。UDP再次被分的后（通常1分为2），只要丢失其中的任意一份，两份都要重新传输。因此，合理的MTU应该是保证数据不被再分的前提下，尽可能的大。

以太网的MTU通常为1500字节-IP头（20字节固定+40字节可选）-UDP头8个字节=1472字节。KCP会考虑多传输协议，但是在UDP的情况下，设置为1472字节更为合理。

2.实际发送

KCP会不停的进行update更新最新情况，数据的实际发送在update时进行。发送过程如下图所示：

![img](https://pic1.zhimg.com/80/v2-2e50a43b3ed2cfd22bdc865e31ec5974_720w.webp)

**步骤一：待发送队列移至发送队列：**

- KCP会把snd_queue待发送队列中的kcp包，移至snd_buf发送队列。移动的包的数量不会超过snd_una+cwnd-snd_nxt，确保发送的数据不会让接收方的接收队列溢出。该功能类似于TCP协议中的滑动窗口。cwnd=min(snd_wnd,rmt_wnd,kcp->cwnd)的最小值决定，snd_wnd，rmt_wnd比较好理解可发送的数据，可发送的数据最大值，应该是发送方可以发送的数据和接收方可以接收的数据的最小值。kcp->cwnd是拥塞控制的一个值，跟网络状况相关，网络状况差的时候，KCP认为应该降低发送的数据，后面会有详细的介绍。
- 如上图中，snd_queue待发送队列中有4个KCP包等待发送，这个时候snd_nxt下一个发送的kcp包序列号为11，snd_una下一个确认的KCP包为9（8已经确认，9，10已经发送但是还没得到接收方的确认）。因为cwnd=5，发送队列中还有2个发送了但是还未得到确认，所以可以从待发送队列中取前面的3个KCP包放入到发送队列中，序列号分别设置为11,12,13。

**步骤二：发送发送队列的数据：**

发送队列中包含两种类型的数据，已发送但是尚未被接收方确认的数据，没被发送过的数据。没发送过的数据比较好处理，直接发送即可。重点在于已经发送了但是还没被接收方确认的数据，该部分的策略直接决定着协议快速、高效与否。 KCP主要使用两种策略来决定是否需要重传KCP数据包，超时重传、快速重传、选择重传。

超时重传：

TCP超时计算是RTOx2，这样连续丢三次包就变成RTOx8了，而KCP非快速模式下每次+RTO，急速模式下+0.5RTO（实验证明1.5这个值相对比较好），提高了传输速度。

![img](https://pic4.zhimg.com/80/v2-851b5a3f238bcb96b0d173f3d5a3e893_720w.webp)

快速重传:

发送端发送了1,2,3,4,5几个包，然后收到远端的ACK: 1, 3, 4, 5，当收到ACK3时，KCP知道2被跳过1次，收到ACK4时，知道2被跳过了2次，此时可以认为2号丢失，不用等超时，直接重传2号包，大大改善了丢包时的传输速度。TCP有快速重传算法，TCP包被跳过3次之后会进行重传。注：可以通过统计错误重传（重传的包实际没丢，仅乱序），优化该设置。

选择重传:

老的TCP丢包时会全部重传从丢的那个包开始以后的数据，KCP是选择性重传，只重传真正丢失的数据包。但是，目前大部分的操作系统，linux与android手机均是支持SACK选择重传的。

**步骤三：数据发送：**

通过步骤2判定，kcp包是否需要发送，如果需要发送的kcp包则通过，kcp_setoutput设置的发送接口进行发送，UDP通常为sendto。步骤3，会对较小的kcp包进行合并，一次性发送提高效率。

3.数据接收过程

KCP的接收过程是将UDP收到的数据进行解包，重新组装顺序的、可靠的数据后交付给用户。

1.KCP数据包接收

kcp_input输入UDP收到的数据包。kcp包对前面的24个字节进行解压，包括conv、 frg、 cmd、 wnd、 ts、 sn、 una、 len。根据una，会删除snd_buf中，所有una之前的kcp数据包，因为这些数据包接收者已经确认。根据wnd更新接收端接收窗口大小。根据不同的命令字进行分别处理。数据接收后，更新流程如下所示：

![img](https://pic3.zhimg.com/80/v2-82471fc151b9e2993925e39d51a717da_720w.webp)

1、IKCP_CMD_PUSH数据发送命令：

a、KCP会把收到的数据包的sn及ts放置在acklist中，两个相邻的节点为一组，分别存储sn和ts。update时会读取acklist，并以IKCP_CMD_ACK的命令返回确认包。如下图中，收到了两个kpc包，acklist中会分别存放10,123,11,124。

b、kcp数据包放置rcv_buf队列。丢弃接收窗口之外的和重复的包。然后将rcv_buf中的包，移至rcv_queue。原来的rcv_buf中已经有sn=10和sn=13的包了，sn=10的kcp包已经在rcv_buf中了，因此新收到的包会直接丢弃掉，sn=11的包放置至rcv_buf中。

c、把rcv_buf中前面连续的数据sn=11，12，13全部移动至rcv_queue，rcv_nxt也变成14。

rcv_queue的数据是连续的，rcv_buf可能是间隔的。d、kcp_recv函数，用户获取接收到数据（去除kcp头的用户数据）。该函数根据frg，把kcp包数据进行组合返回给用户。

![img](https://pic1.zhimg.com/80/v2-08cef05642e908ea4f200481ce2c345c_720w.webp)

2、IKCP_CMD_ACK数据确认包：

两个使命：1、RTO更新，2、确认发送包接收方已接收到。

正常情况：收到的sn为11,una为12。表示sn为11的已经确认，下一个等待接收的为12。发送队列中，待确认的一个包为11，这个时候snd_una向后移动一位，序列号为11的包从发送队列中删除。

![img](https://pic3.zhimg.com/80/v2-e30f58e256e1f0750878c68adba30f3a_720w.webp)

**异常情况：**如下图所示，sn!=11的情况均为异常情况。sn<11表示，收到重复确认的包，如本来以为丢失的包重新又收到了，所以产生重复确认的包；sn>17，收到没发送过的序列号，概率极低，可能是conv没变重启程序导致的；112，则启动快速重传。

![img](https://pic2.zhimg.com/80/v2-1159a3751e5a359e8b1a7f24dc19ceb5_720w.webp)

**确认包发送，接收到的包会全部放在acklist中，以IKCP_CMD_ACK包发送出去。**

## 7.[流量控制](https://link.zhihu.com/?target=https%3A//so.csdn.net/so/search%3Fq%3D%E6%B5%81%E9%87%8F%E6%8E%A7%E5%88%B6%26spm%3D1001.2101.3001.7020)与拥塞控制

### 7.1 RTO计算（与TCP完全一样）

- RTT：一个报文段发送出去，到收到对应确认包的时间差。
- SRTT(kcp->rx_srtt)：RTT的一个加权RTT平均值，平滑值。
- RTTVAR(kcp->rx_rttval)：RTT的平均偏差，用来衡量RTT的抖动。

### 7.2 流量控制

流量控制是点对点的通信量的控制，是一个端到端的问题。总结起来，就是发送方的速度要匹配接收方接收（处理）数据的速度。发送方要抑制自身的发送速率，以便使接收端来得及接收。

KCP的发送机制采用TCP的滑动窗口方式，可以非常容易的控制流量。KCP的头中包含wnd，即接收方目前可以接收的大小。能够发送的数据即为snd_una与snd_una+wnd之间的数据。接收方每次都会告诉发送方我还能接收多少，发送方就控制下，确保自己发送的数据不多于接收端可以接收的大小。

KCP默认为32，即可以接收最大为32*MTU=43.75kB。KCP采用update的方式，更新间隔为10ms，那么KCP限定了你最大传输速率为4375kB/s，在高网速传输大内容的情况下需要调用ikcp_wndsize调整接收与发送窗口。

KCP的主要特色在于实时性高，对于实时性高的应用，如果发生数据堆积会造成延迟的持续增大。建议从应用侧更好的控制发送流量与网络速度持平，避免缓存堆积延迟。

![img](https://pic3.zhimg.com/80/v2-8a920482e817c3752df001419dd22f8e_720w.webp)

### 7.3 拥塞控制（KCP可关闭）

KCP的优势在于可以完全关闭拥塞控制，非常自私的进行发送。KCP采用的拥塞控制策略为TCP最古老的策略，无任何优势。完全关闭拥塞控制，也不是一个最优策略，它全是会造成更为拥堵的情况。

网络中链路的带宽，与整条网络中的交换节点（路由器、交换机、基站等）有关。如果，所有使用该链路的流量超出了，该链路所能提供的能力，就会发生拥塞。车多路窄，就会堵车，车越多堵的越厉害。因此，TCP作为一个大公无私的协议，当网络上发送拥堵的时候会降低自身发送数据的速度。拥塞控制是整个网络的事情，流量控制是发送和接收两方的事情。

当发送方没有按时接收到确认包，就认为网络发生了拥堵行为。TCP拥塞控制的方式，归结为慢开始、拥塞避免，如下图所示:

![img](https://pic4.zhimg.com/80/v2-eb3f5c3cd927110c78bded7e6dde0d13_720w.webp)

KCP发生丢包的情况下的拥塞控制策略与TCP Tahoe版本的策略一致。TCP Reno版本已经使用快恢复策略。因此，丢包的情况下，其实KCP拥塞控制策略比TCP更为苛刻。

KCP在发生快速重传，数据包乱序时，采用的是TCP快恢复的策略。控制窗口调整为已经发送没有接收到ack的数据包数目的一半+resent。

注：目前kernel 3.2以上的linux，默认采用google改进的拥塞控制算法，Proportional Rate Reduction for TCP。该算法的主要特点为，的cwnd如下图所示：

![img](https://pic4.zhimg.com/80/v2-24645bc476b807060ad280a8e20db2ef_720w.webp)

原文地址：https://zhuanlan.zhihu.com/p/581526921

作者：linux