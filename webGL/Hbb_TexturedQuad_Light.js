
//顶点着色器
var VSHADER_SOURCE = 
'attribute float a_type;\n'+
'attribute vec4 a_Position;\n'+
'attribute vec2 a_TexCoord;\n'+
'varying vec2 v_Texcoord;\n'+
'varying float v_Type;\n'+
'uniform mat4 u_ModelMatrix;\n'+
'attribute vec4 a_Normal;\n'+
'varying vec4 v_Normal;\n'+
'void main(){\n'+
'	v_Type = a_type;\n'+
'	if(v_Type == 1.0 ){\n'+
'		gl_Position = u_ModelMatrix * a_Position;\n'+
'		v_Normal = u_ModelMatrix * a_Normal;\n'+
'	}\n'+
'	else{\n'+
'		gl_Position = a_Position;\n'+
'	}\n'+
'	v_Texcoord = a_TexCoord;\n'+
'}\n';

//片元着色器
var FSHADER_SOURCE=
'precision mediump float;\n' +
'uniform sampler2D u_Sampler;\n' +
'uniform sampler2D u_Sampler2;\n' +
'varying vec2 v_Texcoord;\n'+
'varying float v_Type;\n'+
'varying vec4 v_Normal;\n'+
'uniform vec3 u_LightColor;\n'+
'uniform vec3 u_LightDirection;\n'+
'void main(){\n' +
'	if(v_Type == 1.0){\n' +
'		vec4 a_Color = texture2D(u_Sampler ,v_Texcoord);\n'+
'		//对法向量进行归一化 \n'+
'		vec3 normal = normalize(vec3(v_Normal));\n'+
'		//计算光线方向和法向量的点积 \n'+
'		float nDotL = max(dot(u_LightDirection , normal),0.0);\n'+
'		//计算漫反射光的颜色 \n'+
'		vec3 diffuse = u_LightColor * vec3(a_Color)*nDotL;\n'+
'		gl_FragColor = vec4(diffuse , a_Color.a);\n' +
'	}\n' +
'	else{\n' +
'		gl_FragColor = texture2D(u_Sampler2 ,v_Texcoord);\n' +
'	}\n' +
'}\n';

var ANGLE_STEP = 45.0;
var MOVE_STEP = 1.0;
var nCurNum = 1;

function main(){
	//获取<canvas> 元素
	var canvas = document.getElementById('webgl');
	if(!canvas){
		console.log('##Failed to retrieve the <canvas> elemet');
		return;
	}
	
	//获取_xx_绘图上下文
	var gl = getWebGLContext(canvas);
	if(!gl){
		console.log('##Failed to get the rendering context for WebGL ');
		return;
	}
	
	//初始化
	if(!initShaders(gl ,VSHADER_SOURCE ,  FSHADER_SOURCE ) ){
		console.log('##error init');
		return;
	}
	var pModelMatrix = gl.getUniformLocation(gl.program , 'u_ModelMatrix');
	if(pModelMatrix < 0){
		console.log('#error pModelMatrix');
		return;
	}
	
	var pLightColor = gl.getUniformLocation(gl.program , 'u_LightColor');
	var pLightDirection = gl.getUniformLocation(gl.program , 'u_LightDirection');

	//设置光线颜色
	gl.uniform3f(pLightColor , 1.0,1.0,1.0);
	//设置光线方向(世界坐标系)
	var lightDirection = new Vector3([1 , 0 , -4.0]);
	lightDirection.normalize();
	gl.uniform3fv(pLightDirection , lightDirection.elements);
	
	var n = initVertexBuffers(gl);
	if(n < 0){
		console.log('##error n');
		return;
	}
	
	initTextures(gl);
	
	//注册事件
	initEventHandlers(canvas );
	
	//开始_绘制
	var curData = [0.0 , 0.0];
	var modelMatrix = new Matrix4();
	var tick = function(){
		curData = animate(curData[0],curData[1]);
		draw(gl,n , curData , modelMatrix , pModelMatrix ,nCurNum);
		requestAnimationFrame(tick);
	}
	tick();
}

function initEventHandlers(canvas){
	canvas.onmousedown = function(ev){
		var x= ev.clientX;
		if(x < canvas.width/2){
			nCurNum -= 1;
		}
		else{
			nCurNum += 1;
		}
	}
	
	if(nCurNum < 0){
		nCurNum = 0;
	}
	return nCurNum;
}

var max_surface = 7;
function initVertexBuffers(gl){
	var vertices = new Float32Array([
	-1,-1,0.9,				0,0,			0,		0,0,0,
	1,-1,0.9,				1,0,			0,		0,0,0,
	-1,1,0.9,				0,1,			0,		0,0,0,
	1,1,0.9,				1,1,			0,		0,0,0,
	
	-0.125,0.125,0.125,		0,0.5,			1,		0,0,1,
	-0.125,-0.125,0.125,	0,0,			1,		0,0,1,
	0.125,0.125,0.125,		0.25,0.5,		1,		0,0,1,
	0.125,-0.125,0.125,		0.25,0,			1,		0,0,1,
	
	-0.125,0.125,-0.125,	0.25,0.5,		1,		0,0,-1,
	-0.125,-0.125,-0.125,	0.25,0,			1,		0,0,-1,
	0.125,0.125,-0.125,		0.5,0.5,		1,		0,0,-1,
	0.125,-0.125,-0.125,	0.5,0,			1,		0,0,-1,
    
	-0.125,0.125,0.125,		0.5,0.5,		1,		-1,0,0,
	-0.125,-0.125,0.125,	0.5,0,			1,		-1,0,0,
	-0.125,0.125,-0.125,	0.75,0.5,		1,		-1,0,0,
	-0.125,-0.125,-0.125,	0.75,0,			1,		-1,0,0,
	
	0.125,0.125,0.125,		0,1,			1,		1,0,0,
	0.125,-0.125,0.125,		0,0.5,			1,		1,0,0,
	0.125,0.125,-0.125,		0.25,1,			1,		1,0,0,
	0.125,-0.125,-0.125,	0.25,0.5,		1,		1,0,0,
	
	-0.125,0.125,0.125,		0.25,1,			1,		0,1,0,
	0.125,0.125,0.125,		0.25,0.5,		1,		0,1,0,
	-0.125,0.125,-0.125,	0.5,1,			1,		0,1,0,
	0.125,0.125,-0.125,		0.5,0.5,		1,		0,1,0,
	
	-0.125,-0.125,0.125,	0.5,1,			1,		0,-1,0,
	0.125,-0.125,0.125,		0.5,0.5,		1,		0,-1,0,
	-0.125,-0.125,-0.125,	0.75,1,			1,		0,-1,0,
	0.125,-0.125,-0.125,	0.75,0.5,		1,		0,-1,0,
	]);
	var perCount = 9;
	var n = vertices.length/perCount;
	var perSize = vertices.BYTES_PER_ELEMENT;
	
	var vertexBuffer = gl.createBuffer();
	if(!vertexBuffer){
		console.log('##error vertexBuffer');
		return -1;
	}
	
	//绑定_缓冲区
	gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
	
	//编辑_缓冲区
	gl.bufferData(gl.ARRAY_BUFFER ,vertices , gl.STATIC_DRAW);
	

	var pPosition = gl.getAttribLocation(gl.program , 'a_Position');
	if(pPosition < 0){
		console.log('##error , position');
		return -1;
	}
	
	gl.vertexAttribPointer(pPosition , 3,gl.FLOAT,false ,perSize*perCount ,0);
	gl.enableVertexAttribArray(pPosition);
	
	var pTexCoord = gl.getAttribLocation(gl.program, 'a_TexCoord');
	if(pTexCoord < 0){
		console.log("##error pTexCoord");
		return -1;
	}
	gl.vertexAttribPointer(pTexCoord , 2,gl.FLOAT,false ,perSize*perCount ,perSize*3);
	gl.enableVertexAttribArray(pTexCoord);
	
	var pType = gl.getAttribLocation(gl.program, 'a_type');
	if(pType < 0){
		console.log("##error pType");
		return -1;
	}
	gl.vertexAttribPointer(pType , 1,gl.FLOAT,false ,perSize*perCount ,perSize*5);
	gl.enableVertexAttribArray(pType);
	
	var pNormal = gl.getAttribLocation(gl.program, 'a_Normal');
	if(pNormal < 0){
		console.log("##error pNormal");
		return -1;
	}
	gl.vertexAttribPointer(pNormal , 3,gl.FLOAT,false ,perSize*perCount ,perSize*6);
	gl.enableVertexAttribArray(pNormal);
	return n;
}

function initTextures(gl ){
	var pImage =new Image();
	if(!pImage){
		console.log("##error  pImage");
		return -1;
	}
	pImage.onload = function(){	loadTexture(gl , pImage);	};
	pImage.src = 'res/test_all.png';
	
	var pImage2 =new Image();
	if(!pImage2){
		console.log("##error  pImage2");
		return -1;
	}
	pImage2.onload = function(){	loadTexture2(gl , pImage2);	};
	pImage2.src = 'res/back_ground.png';
	
	return 0;
}

function loadTexture(gl ,pImage){
	var pTexture = gl.createTexture();
	if(pTexture < 0){
		console.log("##error pTexture");
		return -1;
	}
	
	var pSampler = gl.getUniformLocation(gl.program , 'u_Sampler');
	if(pSampler < 0){
		console.log("##error  pSampler");
		return -1;
	}
	
	gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL ,1);//纹理_进行Y轴_反转
	
	//开启0号纹理单位
	gl.activeTexture(gl.TEXTURE1);
	
	//绑定纹理
	gl.bindTexture(gl.TEXTURE_2D , pTexture);
	
	//配置_纹理参数
	gl.texParameteri(gl.TEXTURE_2D , gl.TEXTURE_MIN_FILTER , gl.LINEAR );
	
	//配置_纹理图像
	gl.texImage2D(gl.TEXTURE_2D , 0 , gl.RGB , gl.RGB , gl.UNSIGNED_BYTE , pImage);
	
	gl.uniform1i(pSampler, 1);
}

function loadTexture2(gl , pImage2){
	var pTexture2 = gl.createTexture();
	if(pTexture2 < 0){
		console.log("##error pTexture2");
		return -1;
	}
	
	var pSampler2 = gl.getUniformLocation(gl.program , 'u_Sampler2');
	if(pSampler2 < 0){
		console.log("##error  pSampler2");
		return -1;
	}
	gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL ,1);//纹理_进行Y轴_反转

	//开启0号纹理单位
	gl.activeTexture(gl.TEXTURE0);

	//绑定纹理
	gl.bindTexture(gl.TEXTURE_2D , pTexture2);

	//配置_纹理参数
	gl.texParameteri(gl.TEXTURE_2D , gl.TEXTURE_MIN_FILTER , gl.LINEAR );

	//配置_纹理图像
	gl.texImage2D(gl.TEXTURE_2D , 0 , gl.RGB , gl.RGB , gl.UNSIGNED_BYTE , pImage2);

	gl.uniform1i(pSampler2, 0);
}

function draw(gl , n , curData , modelMatrix , pModelMatrix ,nStep ){
	
	modelMatrix.setTranslate( (Math.cos(curData[1])/20)*nStep, (Math.sin(curData[1])/20)*nStep, 0.0);
	modelMatrix.rotate(curData[0], 1 , 1 , 0);
	
	gl.uniformMatrix4fv(pModelMatrix , false , modelMatrix.elements);
	
	gl.enable(gl.DEPTH_TEST);
	gl.clearColor(0,0,0,1);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	
	for(var i = 0 ; i < max_surface ; i++){
		var nBegin = i*4;
		gl.drawArrays(gl.TRIANGLE_STRIP, nBegin, 4 );
	}
}

var g_last = Date.now();
function animate(angle , move1){
	var now = Date.now();
	var elapsed = now - g_last;
	g_last = now;
	var newAngle = angle + (ANGLE_STEP * elapsed) / 1000.0;
	var newMove = move1 + (MOVE_STEP * elapsed) / 1000.0;
	
	return [newAngle %= 360 , newMove%=360];
}







