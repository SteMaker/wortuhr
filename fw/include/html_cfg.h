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
    <p style="font-size:30px">
        {{ time }}
    </p>
    <hr />
    <h2>
        Allgemein
    </h2>
    <p>
        <label>Hostname der Wortuhr</label>
        <input type="text" v-model="hostname">
    </p>
    <hr />
    <h2>
        Zeit
    </h2>
    <p>
        <label>Zeitzone / Stundenversatz</label>
        <input type="number" min="-24" max="+24" v-model="timeZoneOffset">
    </p>
    <p>
        <label>Sommerzeit</label>
        <input type="checkbox" v-model="dayLightSaving">
    </p>
    <hr />
    <h2>
        Farben
    </h2>
    <h3>
        Minuten
    </h3>
    <p>
        <label><span>LED Rotwert (0..255): {{ colorMinutesNumeral.colorRed }}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinutesNumeral.colorRed" v-on:input="onColorChange($event)">
        <label><span>LED Grünwert (0..255): {{ colorMinutesNumeral.colorGreen }}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinutesNumeral.colorGreen" v-on:input="onColorChange($event)">
        <label><span>LED Blauwert (0..255): {{ colorMinutesNumeral.colorBlue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinutesNumeral.colorBlue" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Wort MIN
    </h3>
    <p>
        <label><span>LED Rotwert (0..255): {{ colorMinWord.colorRed }}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinWord.colorRed" v-on:input="onColorChange($event)">
        <label><span>LED Grünwert (0..255): {{ colorMinWord.colorGreen }}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinWord.colorGreen" v-on:input="onColorChange($event)">
        <label><span>LED Blauwert (0..255): {{ colorMinWord.colorBlue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinWord.colorBlue" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Wort VOR/NACH
    </h3>
    <p>
        <label><span>LED Rotwert (0..255): {{ colorPreWord.colorRed }}</span></label>
        <input type="range" min="0" max="255" v-model="colorPreWord.colorRed" v-on:input="onColorChange($event)">
        <label><span>LED Grünwert (0..255): {{ colorPreWord.colorGreen }}</span></label>
        <input type="range" min="0" max="255" v-model="colorPreWord.colorGreen" v-on:input="onColorChange($event)">
        <label><span>LED Blauwert (0..255): {{ colorPreWord.colorBlue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorPreWord.colorBlue" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Wort VIERTEL/HALB/DREIVIERTEL
    </h3>
    <p>
        <label><span>LED Rotwert (0..255): {{ colorQuarterWord.colorRed }}</span></label>
        <input type="range" min="0" max="255" v-model="colorQuarterWord.colorRed" v-on:input="onColorChange($event)">
        <label><span>LED Grünwert (0..255): {{ colorQuarterWord.colorGreen }}</span></label>
        <input type="range" min="0" max="255" v-model="colorQuarterWord.colorGreen" v-on:input="onColorChange($event)">
        <label><span>LED Blauwert (0..255): {{ colorQuarterWord.colorBlue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorQuarterWord.colorBlue" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Stunde
    </h3>
    <p>
        <label><span>LED Rotwert (0..255): {{ colorHoursNumeral.colorRed }}</span></label>
        <input type="range" min="0" max="255" v-model="colorHoursNumeral.colorRed" v-on:input="onColorChange($event)">
        <label><span>LED Grünwert (0..255): {{ colorHoursNumeral.colorGreen }}</span></label>
        <input type="range" min="0" max="255" v-model="colorHoursNumeral.colorGreen" v-on:input="onColorChange($event)">
        <label><span>LED Blauwert (0..255): {{ colorHoursNumeral.colorBlue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorHoursNumeral.colorBlue" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Wort UHR
    </h3>
    <p>
        <label><span>LED Rotwert (0..255): {{ colorClockWord.colorRed }}</span></label>
        <input type="range" min="0" max="255" v-model="colorClockWord.colorRed" v-on:input="onColorChange($event)">
        <label><span>LED Grünwert (0..255): {{ colorClockWord.colorGreen }}</span></label>
        <input type="range" min="0" max="255" v-model="colorClockWord.colorGreen" v-on:input="onColorChange($event)">
        <label><span>LED Blauwert (0..255): {{ colorClockWord.colorBlue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorClockWord.colorBlue" v-on:input="onColorChange($event)">
    </p>
    <hr />
    <h2>
        Helligkeit
    </h2>
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
                time: '00:00',
                hostname: '',
                timeZoneOffset: 0,
                dayLightSaving: false,
                colorMinutesNumeral: {
                    colorRed: 255,
                    colorGreen: 255,
                    colorBlue: 0
                },
                colorMinWord: {
                    colorRed: 0,
                    colorGreen: 0,
                    colorBlue: 255
                },
                colorPreWord: {
                    colorRed: 255,
                    colorGreen: 0,
                    colorBlue: 0
                },
                colorQuarterWord: {
                    colorRed: 255,
                    colorGreen: 255,
                    colorBlue: 255
                },
                colorHoursNumeral: {
                    colorRed: 0,
                    colorGreen: 255,
                    colorBlue: 0
                },
                colorClockWord: {
                    colorRed: 255,
                    colorGreen: 255,
                    colorBlue: 255
                },
                nightOffActive: false,
                nightOffTime: "22:00",
                nightOnTime: "07:00",
                dimActive: false,
                dimBase: 0,
                dimScale: 0,
                timer: ''
            },
            methods: {
                onColorChange() {
                    console.log("changing color");
                    axios.post('/color', {
                        colorMinutesNumeralRed: cfgParams.colorMinutesNumeral.colorRed,
                        colorMinutesNumeralGreen: cfgParams.colorMinutesNumeral.colorGreen,
                        colorMinutesNumeralBlue: cfgParams.colorMinutesNumeral.colorBlue,
                        colorMinWordRed: cfgParams.colorMinWord.colorRed,
                        colorMinWordGreen: cfgParams.colorMinWord.colorGreen,
                        colorMinWordBlue: cfgParams.colorMinWord.colorBlue,
                        colorPreWordRed: cfgParams.colorPreWord.colorRed,
                        colorPreWordGreen: cfgParams.colorPreWord.colorGreen,
                        colorPreWordBlue: cfgParams.colorPreWord.colorBlue,
                        colorQuarterWordRed: cfgParams.colorQuarterWord.colorRed,
                        colorQuarterWordGreen: cfgParams.colorQuarterWord.colorGreen,
                        colorQuarterWordBlue: cfgParams.colorQuarterWord.colorBlue,
                        colorHoursNumeralRed: cfgParams.colorHoursNumeral.colorRed,
                        colorHoursNumeralGreen: cfgParams.colorHoursNumeral.colorGreen,
                        colorHoursNumeralBlue: cfgParams.colorHoursNumeral.colorBlue,
                        colorClockWordRed: cfgParams.colorClockWord.colorRed,
                        colorClockWordGreen: cfgParams.colorClockWord.colorGreen,
                        colorClockWordBlue: cfgParams.colorClockWord.colorBlue
                    })
                        .then(function (response) {
                            console.log(response);
                        })
                        .catch(function (error) {
                            console.log(error);
                       });
                },
                onTimer() {
                axios.get('/timedData')
                    .then((response) => {
                        timeH = response.data.timeH.toString().padStart(2,'0');
                        timeM = response.data.timeM.toString().padStart(2,'0');
                        this.time = timeH+":"+timeM;
                        this.lightIntensity = response.data.lightIntensity.toString();
                    });
                }
            },
            mounted () {
                this.timer = setInterval(this.onTimer, 200);
                this.onTimer();
                axios.get('/config')
                    .then((response) => {
                        noOffH = response.data.nightOffOffHour.toString().padStart(2,'0');
                        noOffM = response.data.nightOffOffMinute.toString().padStart(2,'0');
                        noOnH = response.data.nightOffOnHour.toString().padStart(2,'0');
                        noOnM = response.data.nightOffOnMinute.toString().padStart(2,'0');
                        this.hostname = response.data.hostname;
                        this.timeZoneOffset = response.data.timeZoneOffset;
                        this.dayLightSaving = response.data.dayLightSaving;
                        this.colorMinutesNumeral.colorRed =    response.data.colorMinutesNumeral.colorRed;
                        this.colorMinutesNumeral.colorGreen =  response.data.colorMinutesNumeral.colorGreen;
                        this.colorMinutesNumeral.colorBlue =   response.data.colorMinutesNumeral.colorBlue;
                        this.colorMinWord.colorRed =    response.data.colorMinWord.colorRed;
                        this.colorMinWord.colorGreen =  response.data.colorMinWord.colorGreen;
                        this.colorMinWord.colorBlue =   response.data.colorMinWord.colorBlue;
                        this.colorPreWord.colorRed =    response.data.colorPreWord.colorRed;
                        this.colorPreWord.colorGreen =  response.data.colorPreWord.colorGreen;
                        this.colorPreWord.colorBlue =   response.data.colorPreWord.colorBlue;
                        this.colorQuarterWord.colorRed =    response.data.colorQuarterWord.colorRed;
                        this.colorQuarterWord.colorGreen =  response.data.colorQuarterWord.colorGreen;
                        this.colorQuarterWord.colorBlue =   response.data.colorQuarterWord.colorBlue;
                        this.colorHoursNumeral.colorRed =    response.data.colorHoursNumeral.colorRed;
                        this.colorHoursNumeral.colorGreen =  response.data.colorHoursNumeral.colorGreen;
                        this.colorHoursNumeral.colorBlue =   response.data.colorHoursNumeral.colorBlue;
                        this.colorClockWord.colorRed =    response.data.colorClockWord.colorRed;
                        this.colorClockWord.colorGreen =  response.data.colorClockWord.colorGreen;
                        this.colorClockWord.colorBlue =   response.data.colorClockWord.colorBlue;
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
              colorMinutesNumeralRed: cfgParams.colorMinutesNumeral.colorRed,
              colorMinutesNumeralGreen: cfgParams.colorMinutesNumeral.colorGreen,
              colorMinutesNumeralBlue: cfgParams.colorMinutesNumeral.colorBlue,
              colorMinWordRed: cfgParams.colorMinWord.colorRed,
              colorMinWordGreen: cfgParams.colorMinWord.colorGreen,
              colorMinWordBlue: cfgParams.colorMinWord.colorBlue,
              colorPreWordRed: cfgParams.colorPreWord.colorRed,
              colorPreWordGreen: cfgParams.colorPreWord.colorGreen,
              colorPreWordBlue: cfgParams.colorPreWord.colorBlue,
              colorQuarterWordRed: cfgParams.colorQuarterWord.colorRed,
              colorQuarterWordGreen: cfgParams.colorQuarterWord.colorGreen,
              colorQuarterWordBlue: cfgParams.colorQuarterWord.colorBlue,
              colorHoursNumeralRed: cfgParams.colorHoursNumeral.colorRed,
              colorHoursNumeralGreen: cfgParams.colorHoursNumeral.colorGreen,
              colorHoursNumeralBlue: cfgParams.colorHoursNumeral.colorBlue,
              colorClockWordRed: cfgParams.colorClockWord.colorRed,
              colorClockWordGreen: cfgParams.colorClockWord.colorGreen,
              colorClockWordBlue: cfgParams.colorClockWord.colorBlue,
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