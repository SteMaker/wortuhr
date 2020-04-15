const char htmlCfg[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta name="viewport" content="width=device-width" , initial-scale="1">
    <meta charset="utf-8">
    <title>Wortuhr Konfiguration</title>
    <script src="https://cdn.jsdelivr.net/npm/vue"></script>
    <script src="https://code.jquery.com/jquery-1.11.3.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
    <div id="wortuhrTop" class="title-bar">
        <h1>Konfiguration der Wortuhr {{ ipOfWortuhr }}</h1>
    </div>
    <script>
        var app = new Vue({
            el: '#wortuhrTop',
            data: {
                ipOfWortuhr: ""
            },
            mounted () {
                this.ipOfWortuhr = window.location.host
            }
        })
    </script>
</head>

<body>
    <noscript>
      <strong>Für die Konfiguration der Wortuhr benötigt ihr Browser JavaScript!</strong>
    </noscript>
    <div id="wortuhrCfgMain">
    <p>
        <label>Host name der Wortuhr</label>
        <input type="text" v-model="hostname">
    </p>
    <hr />
    <p>
        <label>Zeitzone / Stundenversatz</label>
        <input type="number" min="-24" max="+24" v-model="timeZoneOffset">
    </p>
    <p>
        <label>Sommerzeit</label>
        <input type="checkbox" v-model="dayLightSaving">
    </p>
    <hr />
    <p>
        <label><span>LED Rotwert (0..255): {{ colorRed }}</span></label>
        <input type="range" min="0" max="255" v-model="colorRed">
    </p>
    <p>
        <label><span>LED Grünwert (0..255): {{ colorGreen }}</span></label>
        <input type="range" min="0" max="255" v-model="colorGreen">
    </p>
    <p>
        <label><span>LED Blauwert (0..255): {{ colorBlue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorBlue">
    </p>
    <input type="button" id="btnColor" value="Testen">
    <hr />
    <p>
        <label>Wortuhr in der Nacht abschalten</label>
        <input type="checkbox" v-model="nightOffActive">
    </p>
    <p v-if="nightOffActive">
        <label>Ausschaltzeit</label>
        <input type="time" v-model="nightOffTime">
    </p>
    <p v-if="nightOffActive">
        <label>Einschaltzeit</label>
        <input type="time" v-model="nightOnTime">
    </p>
    <hr />
    <p>
        <label>Dimmen nach Helligkeit</label>
        <input type="checkbox" v-model="dimActive">
    </p>
    <p v-if="dimActive">
        <label><span>Basiswert bei minimaler Helligkeit: {{ dimBase }}</span></label>
        <input type="range" min="0" max="255" v-model="dimBase">
    </p>
    <p v-if="dimActive">
        <label><span>Skalierung zu maximaler Helligkeit: {{ dimScale }}</span></label>
        <input type="range" min="0" max="255" v-model="dimScale">
    </p>
    <hr />
    <input type="button" id="btnSend" value="Speichern">
    </div>
    <script>
        Vue.config.devtools = true;
        var cfgParams = new Vue({
            el: '#wortuhrCfgMain',
            data: {
                hostname: '',
                timeZoneOffset: 0,
                dayLightSaving: false,
                colorRed: 255,
                colorGreen: 255,
                colorBlue: 255,
                nightOffActive: false,
                nightOffTime: "22:00",
                nightOnTime: "07:00",
                dimActive: false,
                dimBase: 0,
                dimScale: 0
            },
            mounted () {
                axios.get('/config')
                    .then((response) => {
                        noOffH = response.data.nightOffOffHour.toString().padStart(2,'0');
                        noOffM = response.data.nightOffOffMinute.toString().padStart(2,'0');
                        noOnH = response.data.nightOffOnHour.toString().padStart(2,'0');
                        noOnM = response.data.nightOffOnMinute.toString().padStart(2,'0');
                        this.hostname = response.data.hostname;
                        this.timeZoneOffset = response.data.timeZoneOffset;
                        this.dayLightSaving = response.data.dayLightSaving;
                        this.colorRed = response.data.colorRed;
                        this.colorGreen = response.data.colorGreen;
                        this.colorBlue = response.data.colorBlue;
                        this.nightOffActive = response.data.nightOffActive;
                        this.nightOffTime = noOffH + ":" + noOffM;
                        this.nightOnTime = noOnH + ":" + noOnM;
                        this.dimActive = response.data.dimActive;
                        this.dimBase = response.data.dimBase;
                        this.dimScale = response.data.dimScale;
                    });
            }
        });
        $('#btnSend').click(function(){
          var nightOffT = cfgParams.nightOffTime.split(":");
          var nightOnT = cfgParams.nightOnTime.split(":");
          axios.post('/config', {
              hostname: cfgParams.hostname,
              timeZoneOffset: cfgParams.timeZoneOffset,
              dayLightSaving: cfgParams.dayLightSaving,
              colorRed: cfgParams.colorRed,
              colorGreen: cfgParams.colorGreen,
              colorBlue: cfgParams.colorBlue,
              nightOffActive: cfgParams.nightOffActive,
              nightOffOffHour: parseInt(nightOffT[0]),
              nightOffOffMinute: parseInt(nightOffT[1]),
              nightOffOnHour: parseInt(nightOnT[0]),
              nightOffOnMinute: parseInt(nightOnT[1]),
              dimActive: cfgParams.dimActive,
              dimBase: cfgParams.dimBase,
              dimScale: cfgParams.dimScale,
          })
              .then(function (response) {
                  console.log(response);
              })
              .catch(function (error) {
                  console.log(error);
              });
        });
        $('#btnColor').click(function(){
            axios.post('/color', {
              colorRed: cfgParams.colorRed,
              colorGreen: cfgParams.colorGreen,
              colorBlue: cfgParams.colorBlue
            })
                .then(function (response) {
                    console.log(response);
                })
                .catch(function (error) {
                    console.log(error);
            });
        });
    </script
</body>
</html>
)=====";

const char htmlCfgConfirm[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
  <head>
    <title>Wortuhr Konfiguration</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <meta name="msapplication-TileColor" content="#027be3">
    <meta name="theme-color" content="#027be3">
  </head>
  <body>
  <h1>Konfiguration gesetzt</h1>
  </body>
</html>
)=====";

const char htmlCfgReject[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
  <head>
    <title>Wortuhr Konfiguration</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <meta name="msapplication-TileColor" content="#027be3">
    <meta name="theme-color" content="#027be3">
  </head>
  <body>
    <h1>Konfiguration ungültig</h1>
  </body>
</html>
)=====";