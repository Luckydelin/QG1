

# Directx--绘制六边形（图元类型）

# 准备阶段：

流程如下：

![img](https://img-blog.csdnimg.cn/185ec3326edc4156a3b1c659c88d16cb.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_13,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 设备创建准备条件阶段：

```cpp
ComPtr<ID3DBlob> blob;
	//创建顶点着色器
	HR(CreateShaderFromFile(L"HLSL\\VS_Hexagon.cso", L"HLSL\\VS_Hexagon.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf());
	//创建输入布局
	m_pDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf());
	//创建像素着色器
	CreateShaderFromFile(L"HLSL\\PS_Hexagon.cso", L"HLSL\\PS_Hexagon.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf());
	m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf());

    //描述顶点描述区
    D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	//顶点缓冲区数据来源
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	m_pDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf());
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

绑定设备上下文阶段：

```cpp
UINT stride = sizeof(VertexPosColor);	// 跨越字节数
	UINT offset = 0;						// 起始偏移量

	//绑定着色器
	m_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	//设置图元类型
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//设置输入布局
	m_pContext->IASetInputLayout(m_pVertexLayout.Get());
	//绑定顶点缓冲区
	m_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)



# 图元类型：

DirectX11中有多种图元类型，以下图片容易理解图元类型是什么：

![img](https://img-blog.csdnimg.cn/img_convert/d4cef6c2dd56e917cdfdf3dae44b5253.png)

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 无论是最为简单的一维图形还愈加复杂的多维图形，都有点、线、面构成，有了图元，我们就可以通过点、线、三角形以及多边形构成多元的图像了。

以下是一些dx11中可调用的图元类型以及含义：

| 图元类型                                   | 含义                                                         |
| ------------------------------------------ | ------------------------------------------------------------ |
| D3D11_PRIMITIVE_TOPOLOGY_POINTLIST         | 按一系列点进行装配                                           |
| D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP         | 按一系列线段进行装配，每相邻两个顶点(或索引数组相邻的两个索引对应的顶点)构成一条线段 |
| D3D11_PRIMITIVE_TOPOLOGY_LINELIST          | 按一系列线段进行装配，每两个顶点(或索引数组每两个索引对应的顶点)构成一条线段 |
| D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP     | 按一系列三角形进行装配，每相邻三个顶点(或索引数组相邻的三个索引对应的顶点)构成一个三角形 |
| D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST      | 按一系列三角形进行装配，每三个顶点(或索引数组每三个索引对应的顶点)构成一个三角形 |
| D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ      | 每4个顶点为一组，只绘制第2个顶点与第3个顶点的连线（或索引数组每4个索引为一组，只绘制索引模4余数为2和3的连线） |
| D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ     | 绘制除了最开始和结尾的所有线段(或者索引数组不绘制索引0和1的连线，以及n-2和n-1的连线) |
| D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ  | 每6个顶点为一组，只绘制第1、3、5个顶点构成的三角形(或索引数组每6个索引为一组，只绘制索引模6余数为0, 2, 4的三角形) |
| D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ | 抛弃所有索引模2为奇数的顶点或索引，剩余的进行Triangle Strip的绘制 |

## 第一种绘制方式（按图元类型TRIANGLESTRIP）：

设置绘制顶点数：

![img](https://img-blog.csdnimg.cn/472d96986b6749fd806971c55d5231b2.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_16,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)



设置顶点：

![img](https://img-blog.csdnimg.cn/15f036916b914be2bf21bb709fa4858e.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

 设置图元类型：

![img](https://img-blog.csdnimg.cn/a387f5b1b12b4f91b94504799ba8c537.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)





## 第二种绘制方式（按图元类型TRIANGLELIST）：

设置绘制顶点数：

![img](https://img-blog.csdnimg.cn/41847885b0cd4c9db74c63388de7592d.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_14,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)



设置顶点： 

![img](https://img-blog.csdnimg.cn/65cfd994481f4dcebe731d72bc3c5cd8.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

设置图元类型：

![img](https://img-blog.csdnimg.cn/92a882ecfb19490d90f3a57ce9e6bb6c.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)



### 效果图：



![img](https://img-blog.csdnimg.cn/8d87b53734fc4be29399b739e6890c54.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBARnJlYWs1ODc=,size_20,color_FFFFFF,t_70,g_se,x_16)![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

###  总结：

**当我们用TRIANGLESTRIP图元类型的时候，我们需要6个顶点绘制正六边形，而用TRIANGLELIST图元类型的时候，则需要绘制12个顶点。**

因为TRIANGLESTRIP每三个顶点组成三角形后，下一个三角形可以利用前两个顶然后再加上一个顶点就可以绘制另一个三角形了，也就是说，我们可以绘制4个顶点绘制两个三角形。

而TRIANGLELIST图元类型则需要每三个顶点创建一个三角形，就算你绘制了4个顶点，也就可以绘制一个三角形而已。不用说，因为4小于6，而2个三角形需要6个顶点，三个三角形需要12个顶点——

备注：

**顶点的创建必须从顺时针创建。**

一般情况下，三角形顶点从顺时针创建，是面向摄像机的。三角形顶点从逆时针排布则说明当前三角形的面向摄像机。对于背向摄像机的三角形会被剔除，不进行光栅化操作。这种做法叫**背面消隐**