#ifndef WEBPAGE_H
#define WEBPAGE_H

// OTA code
 const char* otaHTML = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<h2 class='title'><center>Обновление прошивки</center></h2>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input class='buttons' type='file' name='update' required>&nbsp;&nbsp;"
        "<input class='buttons' type='submit' value='Обновить'>"
    "</form>"
 "<div id='prg'></div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('Прогресс: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

//код стилей страницы
const char* styleHTML = 
"<style>"
  ".icon-block {position: fixed; top: 1%; right: 2%; font-size: 16px; font-family: sans-serif; font-weight: 700; align-items: center; justify-content: center;}"
  ".mainblock {display: flex; width: 60%; color: white; margin: 10px auto 10px auto; padding:20px; flex-direction: column; align-items: center; justify-content: center; background: Lightcyan;}"
  ".contentblock {width: 90%; margin: 10px auto; background-color: Skyblue; color:black; border-radius:10px; font-size: 16px; font-family: sans-serif; font-weight: 700;}"
  ".title { background-color: Dodgerblue; color: white;margin: 0; padding: 10px; box-sizing: border-box; border-radius: 10px 10px 0 0}"
  ".infotitle { background-color: Forestgreen; color: white; margin: 0; padding: 10px; box-sizing: border-box;}"
  ".warningtitle { background-color: Crimson; color: white; margin: 0; padding: 10px; box-sizing: border-box;}"
  ".content { background-color: Lightcyan; color: black; margin: 0; padding: 10px; border-radius:10px;}"
  ".main {display:block; margin:auto; color: black; background: 	Skyblue; padding: 10px; font-size: 16px; font-family: sans-serif; font-weight: 700;}" 
  ".inputs {padding:10px; border-radius:10px; margin: 10px auto; background: #d3d3d3}"
  ".textareas {padding:10px; border-radius:10px; margin: 10px auto 10px auto; width: 90%; display: flex; background: #d3d3d3}"
  ".buttons {padding:10px;border-radius:10px; background: Dodgerblue; color: white; text-decoration: none; font-size: 16px; font-family: sans-serif; font-weight: 700; align-items: center; justify-content: center; margin: 10px auto 10px auto; border-style:hidden}"
  ".bigbuttons {padding:10px;border-radius:10px; background: Dodgerblue; color: white; text-decoration: none; font-size: 16px; font-family: sans-serif; font-weight: 700; align-items: center; justify-content: center; margin: 10px auto 10px auto; display: flex; width: 98%; border-style:hidden}"
  ".links {padding:10px;border-radius:10px; background: Sandybrown; color: white; text-decoration: none; font-size: 16px; font-family: sans-serif; font-weight: 700; align-items: center; justify-content: center; margin: 10px auto 10px auto; display: flex; width: 88%; }"
  ".select-css {width: 100%; display: block; font-size: 16px; font-family: sans-serif; font-weight: 700; color: #444; line-height: 1.3; padding: .6em 1.4em .5em .8em; width: 100%; max-width: 100%;"
  "box-sizing: border-box; margin: 0; border: 1px solid #aaa; box-shadow: 0 1px 0 1px rgba(0,0,0,.04); border-radius: .5em;"
  "-moz-appearance: none; -webkit-appearance: none; appearance: none; background-color: #fff;" 
  "background-image: url('data:image/svg+xml;charset=US-ASCII,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%22292.4%22%20height%3D%22292.4%22%3E%3Cpath%20fill%3D%22%23007CB2%22%20d%3D%22M287%2069.4a17.6%2017.6%200%200%200-13-5.4H18.4c-5%200-9.3%201.8-12.9%205.4A17.6%2017.6%200%200%200%200%2082.2c0%205%201.8%209.3%205.4%2012.9l128%20127.9c3.6%203.6%207.8%205.4%2012.8%205.4s9.2-1.8%2012.8-5.4L287%2095c3.5-3.5%205.4-7.8%205.4-12.8%200-5-1.9-9.2-5.5-12.8z%22%2F%3E%3C%2Fsvg%3E'), linear-gradient(to bottom, #ffffff 0%,#e5e5e5 100%);"
  "background-repeat: no-repeat, repeat; background-position: right .7em top 50%, 0 0; background-size: .65em auto, 100%;}"
  ".slider {-webkit-appearance: none; appearance: none; width: 98%; height: 25px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s;}"
  ".slider:hover { opacity: 1;}"
  ".slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 8px; background: Dodgerblue; cursor: pointer;}"
  ".slider::-moz-range-thumb {width: 25px; height: 25px; border-radius: 8px; background: Dodgerblue; cursor: pointer;}"  
  ".icon{fill: #000000;}"
  ".checkbox-apple {position: relative; width: 50px; height: 25px; margin: 20px; webkit-user-select: none; -moz-user-select: none;-ms-user-select: none; user-select: none;}"
  ".checkbox-apple label {position: absolute; top: 0; left: 0; width: 50px; height: 25px; border-radius: 50px; background: linear-gradient(to bottom, #b3b3b3, #e6e6e6); cursor: pointer; transition: all 0.3s ease;}"
  ".checkbox-apple label:after {content: ''; position: absolute; top: 1px; left: 1px; width: 23px; height: 23px; border-radius: 50%; background-color: #fff; box-shadow: 0 1px 3px rgba(0, 0, 0, 0.3); transition: all 0.3s ease;}"
  ".checkbox-apple input[type='checkbox']:checked + label {background: linear-gradient(to bottom, #005b98, #1e90ff);}"
  ".checkbox-apple input[type='checkbox']:checked + label:after {transform: translateX(25px);}"
  ".checkbox-apple label:hover {background: linear-gradient(to bottom, #b3b3b3, #e6e6e6);}"
  ".checkbox-apple label:hover:after {box-shadow: 0 1px 3px rgba(0, 0, 0, 0.3);}"
  ".checkbox-apple input {opacity: 0;width: 0; height: 0;}"
  ".yep {position: absolute;top: 0;left: 0;width: 50px;height: 25px;}"
  "</style>";


// penguin favicon.ico
const char* penguinIco = "<link href='data:image/x-icon;base64,AAABAAEAEBAAAAEACABoBQAAFgAAACgAAAAQAAAAIAAAAAEACAAAAAAAAAEAAAAAAAAAAAAAAAEAAAAAAAAAAAAAIhUNADQkMwAFEBUA3O/9ADtDUgBJQTwAV9n/ANrs/AA1JjUAX+brAF3j6gAvM0IAODxLAJXl/gDz6egA5fL9AP///wBa1+AATpH2ADo+TQAXDgsA0+HoAOLy/ACuuMAAVdj/ADYvPAA7QlIAV9j/ANjr/AA6QE8AWdv/ANvu/ABa2/8AEQ8OAIjk/QA2OUgAWd7/AE2Q9gBLREAAW97/ALrFzQBc3v8ABREWAElCPQA2hIwANzVCAGP1/wDV6vwAWuH/AFXX/wDW6vwA9/v9AFvh/wBc4f8A6Pb/AGik9gBb5P8AOT1MADo9TACDh5EA2u38ANvt/ADc7fwA1+r7ADo/TgBi8f8A2ez9AF3b4QAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABklKColBwAAAAAAAAAAKyUjPj4+Pg4lLQAAAAAAACUPPj4+Pj4+ECUAAAAAADULCD4+NDg+Pj4SHAAAAAAlGz4+CCYTFz4+BSUAAAAAJUE+IgY+IBUBPgwlAAAAACU7PicRCD4sET8kJQAAAAAlGjc9Mx0EMENALiUAAAAALzE6PBYUOhgpDTkyAAAAAAAlCjo6Ojo6OkQlAAAAAAAAACUhCR4eAh8lAwAAAAAAAAAAQiUlJSU2AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAP//AAD8PwAA8A8AAOAHAADAAwAAwAMAAIABAACAAQAAgAEAAIABAADAAwAAwAMAAOAHAADwDwAA/D8AAP//AAA=' rel='icon' type='image/x-icon' />";

#endif
