const char file_index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8"/>
    <title>Rolety - test</title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta name="viewport" content="width=device-width,initial-scale=1,height=device-height">
  </head>
  <body>
    <button id="up">nahoru</button>
    <hr>
    <button id="down">dolu</button>
    <hr>
    Testing:<br>
    <select id="servo">
      <option>0</option>
      <option>1</option>
    </select><br>
    <select id="dir">
      <option value="1">Dolu</option>
      <option value="2">nahoru</option>
    </select><br>
    rychlost:
    <input type="number" min="1" max="100" id="speed" value="5"><br>
    <button id="test">Spustit</button>
    
    <script>
      let fetch = async (file = "/", post = false) => {
        return new Promise((resolve, reject) => {
              let request = new XMLHttpRequest();
                  request.open(post ? 'POST' : 'GET', file, true);
                  request.timeout = 10000;
                  request.ontimeout = reject;
                  request.onerror = reject;
                  request.onreadystatechange = function(){
                    if(this.readyState == 4 && this.status == 200){
                      let data = this.responseText;
                      if(data === null || data === '')
                        {
                          data = '{}';
                        }
                      let rObj = {};
                          try {
                              rObj = JSON.parse(data);
                          } catch (e) {
                              reject(e);
                          }
                          resolve(rObj);
                    }
                  };
                  if(post)
                    {
                      request.send(encodeURI(post));
                    }
                  else
                    {
                      request.send();
                    }
            });
      };
      document.getElementById("up").addEventListener("click", e => {
        fetch("/up").then(data => {console.log(data)});
      });
      document.getElementById("down").addEventListener("click", e => {
        fetch("/down").then(data => {console.log(data)});
      });

      document.getElementById("test").addEventListener("click", e => {
        fetch(`/test?s=${document.getElementById("servo").value}&d=${document.getElementById("dir").value}&sp=${document.getElementById("speed").value}`)
        .then(data => {console.log(data)});
      });
    </script>
  </body>
</html>
)=====";
