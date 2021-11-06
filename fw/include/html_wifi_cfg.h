const char htmlCfgWifi[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
  <head>
    <title>WORTUHR WiFi Konfiguration</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <link rel="icon" 
          type="image/x-icon" 
          href="data:image/x-icon;base64,AAABAAEAEBAQAAEABAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAA9/8AAAAAAH9/fwD///8AAP8AAAAA/wD//wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAERERERERERERERERERERERISEhITExMREREREREREREUFBQUFBISEREREREREREREhISEhISEhERERERERERERISEhUVFRUREREREREREREWFhYSEhISEREREREREREREhISEhISEhERERERERERERISEhAQEBAREREREREREREAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"/>
  </head>
  <body>
  <noscript>
    <strong>Für die Konfiguration der Wortuhr benötigt ihr Browser JavaScript!</strong>
  </noscript>

  <form action="" method="post">
    <label for="ssid">SSID Ihres WLANs:</label>
    <input type="text" id="ssid" name="ssid"><br><br>
    <label for="passwd">Passwort Ihres WLANs:</label>
    <input type="text" id="passwd" name="passwd"><br><br>
    <input type="submit" value="Submit">
  </form>

  <script>
    function logErr() {
      var el = document.getElementById('err');
      el.innerHTML = el.innerHTML + "<p><strong>Error: </strong>"
          + (this.src || this.href)
          + " konnte nicht geladen werden. <br>"
          + "Internet Einstellungen und Werbeblocker überprüfen.</p>"
    }
  </script>
  </body>
</html>
)=====";

const char htmlCfgWifiConfirm[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
  <head>
    <title>WORTUHR WiFi Konfiguration</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <meta name="msapplication-TileColor" content="#027be3">
    <meta name="theme-color" content="#027be3">
  </head>
  <body>
  <h1>Konfiguration gesetzt</h1>
  Wortuhr wird jetzt neu gestartet und sollte sich in das lokale WLAN einwählen
  </body>
</html>
)=====";

const char htmlCfgWifiReject[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
  <head>
    <title>WORTUHR WiFi Konfiguration</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <meta name="msapplication-TileColor" content="#027be3">
    <meta name="theme-color" content="#027be3">
  </head>
  <body>
  <h1>Konfiguration ungültig</h1>
  Wortuhr wird jetzt neu gestartet und sollte sich wieder als Access Point melden
  </body>
</html>
)=====";