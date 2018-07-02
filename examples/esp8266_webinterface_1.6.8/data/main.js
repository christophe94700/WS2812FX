
function submitVal(name, val) {
  var xhttp = new XMLHttpRequest();
  xhttp.open('GET', 'set?' + name + '=' + val, true);
  xhttp.send();
  if ((name=='p') & (val=='m')){
  document.getElementById("m").style.color="green";
  document.getElementById("a").style.color="#454545";
  
  }
  if ((name=='p') & (val=='a')){
  document.getElementById("m").style.color="#454545";
  document.getElementById("a").style.color="red";
  
  }
}
 function rafraichir() { 
 var date = new Date(),secondes = date.getSeconds(); 
 
 /* Fonction ' SECONDES ' */
 if (secondes <= 9) {
 secondes = '0' + secondes;
 }
 
 ntp();
 var CheminComplet = document.location.href;
 var NomDuFichier = CheminComplet.substring(CheminComplet.lastIndexOf( "/" )+1 );
 if ((NomDuFichier =="index.html") | (NomDuFichier =="")){
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