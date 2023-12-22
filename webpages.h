/*
Скрипты, графические ресурсы и стили для веб интерфейса
*/
#ifndef WEBPAGE_H
#define WEBPAGE_H

// OTA code
 const char* otaHTML = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<h2 class='title'><center>Обновление прошивки</center></h2>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "&nbsp;&nbsp;<input class='buttons' type='file' name='update' required>&nbsp;&nbsp;"
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
  ".icon-block {position: fixed; top: 1%; right: 2%; font-size: 22px; font-family: sans-serif; font-weight: 700; color: black;}"
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


// favicon.ico
const char* favincon = "<link href='data:image/x-icon;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAFJGlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgOS4xLWMwMDEgNzkuYThkNDc1MywgMjAyMy8wMy8yMy0wODo1NjozNyAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iIHhtbG5zOnBob3Rvc2hvcD0iaHR0cDovL25zLmFkb2JlLmNvbS9waG90b3Nob3AvMS4wLyIgeG1sbnM6eG1wTU09Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9tbS8iIHhtbG5zOnN0RXZ0PSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvc1R5cGUvUmVzb3VyY2VFdmVudCMiIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIDI1LjAgKDIwMjMwNzI1Lm0uMjI1NCA5ZDJlZTk4KSAgKFdpbmRvd3MpIiB4bXA6Q3JlYXRlRGF0ZT0iMjAyMy0xMi0yMFQxMToxMjoxNiswMzowMCIgeG1wOk1vZGlmeURhdGU9IjIwMjMtMTItMjBUMTE6MTU6MzQrMDM6MDAiIHhtcDpNZXRhZGF0YURhdGU9IjIwMjMtMTItMjBUMTE6MTU6MzQrMDM6MDAiIGRjOmZvcm1hdD0iaW1hZ2UvcG5nIiBwaG90b3Nob3A6Q29sb3JNb2RlPSIzIiB4bXBNTTpJbnN0YW5jZUlEPSJ4bXAuaWlkOjM0Nzk2ZDhhLTQ1MDctODk0Zi1hZDMxLWYxY2Ez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' rel='icon' type='image/x-icon' />";

#endif
