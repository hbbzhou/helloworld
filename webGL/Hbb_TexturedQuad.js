
//顶点着色器
var VSHADER_SOURCE = 
'attribute vec4 a_Position;\n'+
'attribute vec2 a_TexCoord;\n'+
'varying vec2 v_Texcoord;\n'+
'uniform mat4 u_ModelMatrix;\n'+
'void main(){\n'+
'	gl_Position = u_ModelMatrix * a_Position;\n'+
'	v_Texcoord = a_TexCoord;\n'+
'}\n';

//片元着色器
var FSHADER_SOURCE=
'precision mediump float;\n' +
'uniform sampler2D u_Sampler;\n' +
'varying vec2 v_Texcoord;\n'+
'void main(){\n' +
'	gl_FragColor = texture2D(u_Sampler ,v_Texcoord);\n' +
'}\n';

var ANGLE_STEP = 45.0;
var MOVE_STEP = 1.0;

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
	
	var n = initVertexBuffers(gl);
	if(n < 0){
		console.log('##error n');
		return;
	}
	
	initTextures(gl , n);
	//开始_绘制
	
	var curData = [0.0 , 0.0];
	var modelMatrix = new Matrix4();
	var tick = function(){
		curData = animate(curData[0],curData[1]);
		draw(gl,n , curData , modelMatrix , pModelMatrix );
		requestAnimationFrame(tick);
	}
	tick();

}

function initVertexBuffers(gl){
	var vertices = new Float32Array([
	-0.2,0.2,0.2,		0,0.5,
	-0.2,-0.2,0.2,		0,0,
	0.2,0.2,0.2,		0.25,0.5,
	0.2,-0.2,0.2,		0.25,0,
	
	-0.2,0.2,-0.2,		0.25,0.5,
	-0.2,-0.2,-0.2,		0.25,0,
	0.2,0.2,-0.2,		0.5,0.5,
	0.2,-0.2,-0.2,		0.5,0,

	-0.2,0.2,0.2,		0.5,0.5,
	-0.2,-0.2,0.2,		0.5,0,
	-0.2,0.2,-0.2,		0.75,0.5,
	-0.2,-0.2,-0.2,		0.75,0,
	
	0.2,0.2,0.2,		0,1,
	0.2,-0.2,0.2,		0,0.5,
	0.2,0.2,-0.2,		0.25,1,
	0.2,-0.2,-0.2,		0.25,0.5,
	
	-0.2,0.2,0.2,		0.25,1,
	0.2,0.2,0.2,		0.25,0.5,
	-0.2,0.2,-0.2,		0.5,1,
	0.2,0.2,-0.2,		0.5,0.5,
	
	-0.2,-0.2,0.2,		0.5,1,
	0.2,-0.2,0.2,		0.5,0.5,
	-0.2,-0.2,-0.2,		0.75,1,
	0.2,-0.2,-0.2,		0.75,0.5,
	]);
	var perCount = 5;
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
	
	return n;
}

function initTextures(gl , n){
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
	
	var pImage =new Image();
	if(!pImage){
		console.log("##error  pImage");
		return -1;
	}
	
	pImage.onload = function(){	loadTexture(gl , n , pTexture , pSampler , pImage);	};
	pImage.src = 'res/test_all.png';
	
	return 0;
	
}

function loadTexture(gl , n , pTexture , pSampler , pImage){
	gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL ,1);//纹理_进行Y轴_反转
	
	//开启0号纹理单位
	gl.activeTexture(gl.TEXTURE0);
	//绑定纹理
	gl.bindTexture(gl.TEXTURE_2D , pTexture);
	
	//配置_纹理参数
	gl.texParameteri(gl.TEXTURE_2D , gl.TEXTURE_MIN_FILTER , gl.LINEAR );
	
	//配置_纹理图像
	gl.texImage2D(gl.TEXTURE_2D , 0 , gl.RGB , gl.RGB , gl.UNSIGNED_BYTE , pImage);
	
	gl.uniform1i(pSampler, 0);
}

function draw(gl , n , curData , modelMatrix , pModelMatrix ){
	modelMatrix.setTranslate( Math.cos(curData[1])/2, Math.sin(curData[1])/2, 0.0);
	modelMatrix.rotate(curData[0], 1 , 1 , 0);
	
	gl.uniformMatrix4fv(pModelMatrix , false , modelMatrix.elements);
	
	gl.enable(gl.DEPTH_TEST);
	gl.clearColor(0,0,0,1);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	
	gl.drawArrays(gl.TRIANGLES, 0, n);
	for(var i = 0 ; i < 6 ; i++){
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







