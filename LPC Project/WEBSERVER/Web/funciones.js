var value_bars = new Array(8);
var type_bars = new Array(8);
var num_bars = 0;
var num_texts = 0;
var BAR_RANGE = 255;
var ratonx = 0;
var ratony = 0;
var screenX = 200;
var screenY = 150;
var screenW = 320;
var screenH = 240;
var textos = new Array(8);
	textos[0] = "R";
	textos[1] = "G";
	textos[2] = "B";
	
	textos[3] = "B";
	textos[4] = "C";
	textos[5] = "L";
	
function coordenadas(event) {
 	ratonx = event.pageX;
 	ratony = event.pageY;
}
 
function Draw_detected(detX,detY) {
	document.getElementById("div1").style.left = (detX + screenX).toString()+"px";	
	document.getElementById("div1").style.top = (detY + screenY).toString()+ "px";	
 } 

function getBarValue(elemento,tipo){
	anchura = parseInt(window.getComputedStyle(elemento).getPropertyValue('width'));
	altura = parseInt(window.getComputedStyle(elemento).getPropertyValue('height'));
	origenx = parseInt(window.getComputedStyle(elemento).getPropertyValue('left'));
	origeny =  parseInt(window.getComputedStyle(elemento).getPropertyValue('top'));
	
	aux_num_bar =parseInt(elemento.name);
	
	if (tipo == 0){ valor = BAR_RANGE*(ratonx - origenx)/anchura;}
	if (tipo == 1){valor = BAR_RANGE*(ratony - origeny)/altura;}
	if (valor > BAR_RANGE) valor = BAR_RANGE;
	
	Valor_bar = valor;
	value_bars[aux_num_bar] = parseInt(Valor_bar);
	
	
	return valor;
}

function DrawBar(elemento,valor,tipo) {

	lleno = elemento.getElementsByTagName("div")[0];
	vacio = elemento.getElementsByTagName("div")[1];

	anchura = parseInt(window.getComputedStyle(elemento).getPropertyValue('width'));
	altura = parseInt(window.getComputedStyle(elemento).getPropertyValue('height'));
	
	if (tipo == 0){
		valor = anchura*valor/BAR_RANGE;
		lleno.style.height = altura.toString()+"px";
		vacio.style.height = altura.toString()+"px";
		
		vacio.style.left = valor.toString()+"px";	
		lleno.style.width = valor.toString()+"px";
		vacio.style.width = (anchura-valor).toString()+"px";
	}
	if (tipo == 1){
		valor = altura*valor/BAR_RANGE;	
		vacio.style.top = valor.toString()+"px";	
		lleno.style.height = valor.toString()+"px";
		vacio.style.height = (altura-valor).toString()+"px";
		
		lleno.style.width = anchura.toString()+"px";
		vacio.style.width = anchura.toString()+"px";
	}	
}

function CreateBar(x,y,w,h,tipo) {
	CreateInp (x +20,y+20,"barra_inp" + num_bars.toString(),value_bars[num_bars]); 
	
	var barra = document.createElement("div");

	barra.id = "barra" + num_bars.toString();
	barra.name = num_bars.toString();

	barra.innerHTML = "<div class='lleno'></div>"+
					 "<div class='vacio'></div>";
	
	barra.style.position = "absolute";	
	barra.style.left = x.toString()+"px";
	barra.style.top = y.toString()+"px";
	barra.style.height = h.toString()+"px";
	barra.style.width = w.toString()+"px";

	type_bars[num_bars] = tipo;
			
	document.super_form.appendChild(barra);
	DrawBar(barra,value_bars[num_bars],tipo);		
	num_bars++;
} 

function CreateDiv(x,y,str) {
	var texto = document.createElement("div");
	texto.id = "div" + num_texts.toString();
	texto.name = num_bars.toString();
	texto.innerHTML = str;
	texto.style.position = "absolute";	
	texto.style.left = x.toString()+"px";
	texto.style.top = y.toString()+"px";

	document.super_form.appendChild(texto);  
	num_texts++;
} 


function CreateInp (x,y,name,value) {
	var inputcillo = document.createElement("input");
	inputcillo.name = name;
	inputcillo.id = name;
	inputcillo.type = "text";	
	
	inputcillo.style.position = "absolute";	
	inputcillo.style.left = x.toString()+"px";
	inputcillo.style.top = y.toString()+"px";
	inputcillo.hidden = 1;
	
	inputcillo.value = value;	
	document.super_form.appendChild(inputcillo); 	
	
} 

function CreateInp_send (x,y,name,value) {
	var inputcillo = document.createElement("input");
	inputcillo.name = name;
	inputcillo.id = name;
	inputcillo.type = "submit";	
	
	inputcillo.style.position = "absolute";	
	inputcillo.style.left = x.toString()+"px";
	inputcillo.style.top = y.toString()+"px";
	inputcillo.hidden = 0;
	
	inputcillo.value = value;	
	document.super_form.appendChild(inputcillo); 	
	
} 


 
