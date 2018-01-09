window.addEventListener('load', setup);

function handle_M_B_S(e) {
  var CheminComplet = document.location.href;
  var NomDuFichier = CheminComplet.substring(CheminComplet.lastIndexOf( "/" )+1 );
  if (NomDuFichier =="config.html"){
  e.preventDefault();
  var name = e.target.className;
  var val = e.target.id;
  if(e.target.className.indexOf('m') > -1) {
    elems = document.querySelectorAll('#mode li a');
    [].forEach.call(elems, function(el) {
      el.classList.remove('active');
      name = e.target.className;
    });
    e.target.classList.add('active');
  }
  submitVal(name, val);
  }
}
// Envoie requête
function submitVal(name, val) {
  var xhttp = new XMLHttpRequest();
  xhttp.open('GET', 'set?' + name + '=' + val, true);
  xhttp.send();
}

function setup(){
  var CheminComplet = document.location.href;
  var NomDuFichier = CheminComplet.substring(CheminComplet.lastIndexOf( "/" )+1 );
  if (NomDuFichier =="config.html"){
 
 // Affichage valeur minuteur
	var xhr = new XMLHttpRequest();
	xhr.open('GET', 'set?vta=0', true);
	xhr.responseType = 'text';
	xhr.onload = function () {
		if (xhr.readyState === xhr.DONE) {
			if (xhr.status === 200) {
			document.getElementById("timer").value =xhr.responseText;
			}
		}
	};
	xhr.send(null);

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
      document.getElementById('mode').innerHTML = xhttp.responseText;
      elems = document.querySelectorAll('ul li a'); // adds listener also to existing s and b buttons
      [].forEach.call(elems, function(el) {
        el.addEventListener('click', handle_M_B_S, false);
      });
    }
  };
  xhttp.open('GET', 'modes', true);
  xhttp.send();
 }
}

function rafraichir() {             
           ntp();
		   var CheminComplet = document.location.href;
		   var NomDuFichier = CheminComplet.substring(CheminComplet.lastIndexOf( "/" )+1 );
		   if ((NomDuFichier =="index.html") ||(NomDuFichier =="")){
		        minuterie();
				}
}

setInterval(rafraichir, 1000);

function ntp() {
	var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
      document.getElementById('dateheure').innerHTML = xhttp.responseText;
    }
  };
  xhttp.open('GET', 'DateHeure', true);
  xhttp.send();
}
function minuterie() {
	var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
      document.getElementById('minuterie').innerHTML = xhttp.responseText;
    }
  };
  xhttp.open('GET', 'Minuteur', true);
  xhttp.send();
}

function update(jscolor) {
    // 'jscolor' instance can be used as a string
    document.getElementById('ValeurCol').value = jscolor;
    
}
function mouseUp() {
    // 'jscolor' instance can be used as a string
   
    if (document.getElementById('bouton').value=="Validation de la couleur")
    {
    document.getElementById('bouton').value = "Choix de la couleur";
     document.getElementById('bouton').jscolor.hide();
     submitVal('c', document.getElementById('ValeurCol').value);
    }
    else
    {
    document.getElementById('bouton').value = "Validation de la couleur";
    
    }
   }
// Mise à zéro de la minuterie 
function raz(){
submitVal('ta',(0));
document.getElementById("timer").value ="0";

}
