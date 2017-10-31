const PROGMEM char* const index_html = R"=====(
<!DOCTYPE html>
<html lang='en'>
<head>
  <meta http-equiv='Content-Type' content='text/html; charset=utf-8' />
  <meta name='viewport' content='width=device-width' />

  <title>WS2812FX FR Ctrl</title>

  <script type='text/javascript' src='main.js'></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.js"></script>

  <style>
  * {
    font-family:sans-serif;
    margin:0;
    padding:0;
  }

  body {
    max-width:675px;
    min-width:250px;
    background-color:#202020;
    margin-left: 10%;
    margin-right:10%;
  }
  
  h1 {
    color:#454545;
    text-align:center;
  }
  
  #controls {
    text-align:center;
    width:100%;
    display:inline-block;
  }

  ul {
    text-align:center;
  }

  ul#mode li {
    display:block;
  }

  ul#brightness li, ul#speed li {
    display:inline-block;
    width:30%;
  }

  ul li a {
    display:block;
    margin:3px;
    padding:10px 5px;
    border:2px solid #454545;
    border-radius:5px;
    color:#454545;
    font-weight:bold;
    text-decoration:none;
  }

  ul li a.active {
    border:2px solid #909090;
    color:#909090;
  }

  .dropbtn {
   display:block;
    margin:3px;
    padding: 16px;
    font-size: 16px;
    border:2px solid #ffffff;
   background-color:#202020;
    font-weight:bold;
    text-decoration:none;
    cursor: pointer;
    min-width: 240px;
    color:#454545;
    border-radius:5px;
}

.dropdown {
    position: relative;
    display: inline-block;
}

.dropdown-content {
    display: none;
    position: absolute;
    background-color:#202020;
    color:#454545;
    max-height:400px;
    overflow-y: scroll;

}

.dropdown-content a {
    display:block;
    margin:3px;
    padding:10px;
    border:2px solid #454545;
    border-radius:5px;
    color:#454545;
    font-weight:bold;
    text-decoration:none;
    cursor: pointer;
    min-width: 218px;

}

.dropdown-content a:hover {background-color: #909090}

.dropdown:hover .dropdown-content {
    display: block;
}
  </style>
</head>
<body>
  <h1>WS2812FX FR Control</h1>
  <input type="hidden" id="ValeurCol" onchange=change(value)  value="Exemple"  /><br />
  
  <div id='controls'>
      <ul id='brightness'>
      <li><a href='#' class='b' id='-'>&#9788;</a></li>
      <li><a href='#' class='b' id='+'>&#9728;</a></li>
    </ul>

    <ul id='speed'>
      <li><a href='#' class='s' id='-'>&#8722;</a></li>
      <li><a href='#' class='s' id='+'>&#43;</a></li>
    </ul>
    <br />
    <input type="button"
    class="jscolor {onFineChange:'update(this)', valueElement:null,value:'66ccff'}"
    id="bouton"
    value="Choix de la couleur"
    onclick="mouseUp()"
    style="width:240px; height:54px;"><br /><br />

    <div class="dropdown">
    <button class="dropbtn">Mode du bandeau</button>
      <div class="dropdown-content"> 
      <ul id='mode'></ul>
      </div>
    </div>
  </div>
</body>
</html>
)=====";
