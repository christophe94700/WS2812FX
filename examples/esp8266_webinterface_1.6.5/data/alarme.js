window.onload=function(){

function clicked(){

	var cl = this.className;
	if (this.id) {
	if (/selected/.test(cl)) {
		this.className = cl.replace(" selected", "");
		selected=removeByValue(selected, this.innerHTML);
	}else {
		this.className += " selected";
		selected.push(this.innerHTML);

	}
	console.log(selected[0]);
	console.log(selected[1]);
	console.log(selected[2]);
	console.log(selected[3]);
	console.log(selected[4]);
	}
}

function valider(e){
	e.preventDefault();
	
	var i = 0,
		l = selected.length;
	
	for( ; i<l; i+=1) {
	
	envoie="";
	num=0;
	z=0;
	// Sélection des nombres
	for ( ; z<2; z++){ 
	if ((document.getElementById(selected[i]+z).value)!=null) envoie=envoie+document.getElementById(selected[i]+z).value;
	envoie=envoie+",";
	}
	// Sélection du Marche arrêt
	num=1-(ListIndex("marche", (selected[i]+2)));
	envoie=envoie+num+",";
	// Sélection du jour
	num=ListIndex("jour", (selected[i]+3));
	envoie=envoie+num+",";
	// Sélection de la couleur
	num=ListIndex("couleur", (selected[i]+4));
	envoie=envoie+num+",";
	// Sélection du mode
	num = ListIndex("modes", (selected[i]+5));
	envoie=envoie+num;
	valeur=selected[i].split(" ");
    console.log((valeur[1]-1)+","+envoie);
	submitVal("tal", (((valeur[1]-1).toString())+","+envoie));
	}
	
	l == 0 && alert( "Vous n'avez rien choisi :(");
}

var tr = document.querySelectorAll("#table td "),
	a = document.querySelector("a"),
	selected = [],
	i = 0,
	l = tr.length;

for ( ; i<l; i++) tr[i].addEventListener("click", clicked, false);

a.addEventListener("click", valider, false);

function ListIndex(Liste, Input) {
	// Index dans liste de choix
	datalist = document.getElementById (Liste);
	input = document.getElementById (Input);
	for (var j=0;j<datalist.options.length;j++){
    if (datalist.options[j].value == input.value){ 
	console.log(j);
	break;}
	}
	return j
}

function submitVal(name, val) {
  var xhttp = new XMLHttpRequest();
  xhttp.open('GET', 'set?' + name + '=' + val, true);
  xhttp.send();
}
function removeByValue(array, value){
    return array.filter(function(elem, _index){
        return value != elem;
    });
}
function enforce_maxlength(event) {
  var t = event.target;
  if (t.hasAttribute('maxlength')) {
    t.value = t.value.slice(0, t.getAttribute('maxlength'));
  }
}

// Global Listener for anything with an maxlength attribute.
// I put the listener on the body, put it on whatever.
document.body.addEventListener('input', enforce_maxlength);
}