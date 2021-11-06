const char htmlCfg[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta name="viewport" content="width=device-width" , initial-scale="1">
    <meta charset="utf-8">
    <link rel="icon" 
          type="image/x-icon" 
          href="data:image/x-icon;base64,AAABAAEAEBAQAAEABAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAA9/8AAAAAAH9/fwD///8AAP8AAAAA/wD//wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAERERERERERERERERERERERISEhITExMREREREREREREUFBQUFBISEREREREREREREhISEhISEhERERERERERERISEhUVFRUREREREREREREWFhYSEhISEREREREREREREhISEhISEhERERERERERERISEhAQEBAREREREREREREAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"/>
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
    <p>
        <label>IP-Adresse beim Start anzeigen:</label>
        <input type="checkbox" v-model="showIp">
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
        <select v-model="dayLightSaving">
            <option v-for="option in dstOptions" v-bind:value="option.value">
                {{ option.text }}
            </option>
        </select>
        <span>Sommerzeit: {{ dayLightSaving }}</span>
    </p>
    <hr />
    <h2>
        Farben
    </h2>
    <h3>
        LED Grundhelligkeit
    </h3>
    <p>
        <label><span>LED Helligkeit(0..255): {{ luma }}</span></label>
        <input type="range" min="0" max="255" v-model="luma" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Minuten
    </h3>
    <p>
        <label><span>LED Farbwert (0..255): {{ colorMinutesNumeral.hue}}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinutesNumeral.hue" v-on:input="onColorChange($event)">
        <label><span>LED Sättigung (0..255): {{ colorMinutesNumeral.sat}}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinutesNumeral.sat" v-on:input="onColorChange($event)">
        <label><span>LED Helligkeitsanpassung(-64..64): {{ colorMinutesNumeral.lumaOffset}}</span></label>
        <input type="range" min="-64" max="64" v-model="colorMinutesNumeral.lumaOffset" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Wort MIN
    </h3>
    <p>
        <label><span>LED Farbwert (0..255): {{ colorMinWord.hue}}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinWord.hue" v-on:input="onColorChange($event)">
        <label><span>LED Sättigung(0..255): {{ colorMinWord.sat}}</span></label>
        <input type="range" min="0" max="255" v-model="colorMinWord.sat" v-on:input="onColorChange($event)">
        <label><span>LED Helligkeitsanpassung(-64..64): {{ colorMinWord.lumaOffset}}</span></label>
        <input type="range" min="-64" max="64" v-model="colorMinWord.lumaOffset" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Wort VOR/NACH
    </h3>
    <p>
        <label><span>LED Farbwert (0..255): {{ colorPreWord.hue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorPreWord.hue" v-on:input="onColorChange($event)">
        <label><span>LED Sättigung (0..255): {{ colorPreWord.sat }}</span></label>
        <input type="range" min="0" max="255" v-model="colorPreWord.sat" v-on:input="onColorChange($event)">
        <label><span>LED Helligkeitsanpassung(-64..64): {{ colorPreWord.lumaOffset}}</span></label>
        <input type="range" min="-64" max="64" v-model="colorPreWord.lumaOffset" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Wort VIERTEL/HALB/DREIVIERTEL
    </h3>
    <p>
        <label><span>LED Farbwert (0..255): {{ colorQuarterWord.hue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorQuarterWord.hue" v-on:input="onColorChange($event)">
        <label><span>LED Sättigung (0..255): {{ colorQuarterWord.sat }}</span></label>
        <input type="range" min="0" max="255" v-model="colorQuarterWord.sat" v-on:input="onColorChange($event)">
        <label><span>LED Helligkeitsanpassung(-64..64): {{ colorQuarterWord.lumaOffset }}</span></label>
        <input type="range" min="-64" max="64" v-model="colorQuarterWord.lumaOffset" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Stunde
    </h3>
    <p>
        <label><span>LED Farbwert (0..255): {{ colorHoursNumeral.hue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorHoursNumeral.hue" v-on:input="onColorChange($event)">
        <label><span>LED Sättigung (0..255): {{ colorHoursNumeral.sat }}</span></label>
        <input type="range" min="0" max="255" v-model="colorHoursNumeral.sat" v-on:input="onColorChange($event)">
        <label><span>LED Helligkeitsanpassung(-64..64): {{ colorHoursNumeral.lumaOffset }}</span></label>
        <input type="range" min="-64" max="64" v-model="colorHoursNumeral.lumaOffset" v-on:input="onColorChange($event)">
    </p>
    <h3>
        Wort UHR
    </h3>
    <p>
        <label><span>LED Farbwert (0..255): {{ colorClockWord.hue }}</span></label>
        <input type="range" min="0" max="255" v-model="colorClockWord.hue" v-on:input="onColorChange($event)">
        <label><span>LED Sättigung (0..255): {{ colorClockWord.sat}}</span></label>
        <input type="range" min="0" max="255" v-model="colorClockWord.sat" v-on:input="onColorChange($event)">
        <label><span>LED Helligkeitsanpassung(-64..64): {{ colorClockWord.lumaOffset }}</span></label>
        <input type="range" min="-64" max="64" v-model="colorClockWord.lumaOffset" v-on:input="onColorChange($event)">
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
                showIp: false,
                timeZoneOffset: 0,
                dayLightSaving: 2,
                dstOptions: [
                    { text: 'nein', value: 0 },
                    { text: 'ja',   value: 1 },
                    { text: 'auto', value: 2 }
                ],
                luma: 255,
                colorMinutesNumeral: {
                    hue: 42,
                    sat: 255,
                    lumaOffset: 0
                },
                colorMinWord: {
                    hue: 170,
                    sat: 255,
                    lumaOffset: 0
                },
                colorPreWord: {
                    hue: 0,
                    sat: 255,
                    lumaOffset: 0 
                },
                colorQuarterWord: {
                    hue: 0,
                    sat: 0,
                    lumaOffset: 0
                },
                colorHoursNumeral: {
                    hue: 85,
                    sat: 255,
                    lumaOffset: 0
                },
                colorClockWord: {
                    hue: 0,
                    sat: 0,
                    lumaOffset: 0
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
                        luma: cfgParams.luma,
                        colorMinutesNumeralHue: cfgParams.colorMinutesNumeral.hue,
                        colorMinutesNumeralSat: cfgParams.colorMinutesNumeral.sat,
                        colorMinutesNumeralLumaOffset: cfgParams.colorMinutesNumeral.lumaOffset,
                        colorMinWordHue: cfgParams.colorMinWord.hue,
                        colorMinWordSat: cfgParams.colorMinWord.sat,
                        colorMinWordLumaOffset: cfgParams.colorMinWord.lumaOffset,
                        colorPreWordHue: cfgParams.colorPreWord.hue,
                        colorPreWordSat: cfgParams.colorPreWord.sat,
                        colorPreWordLumaOffset: cfgParams.colorPreWord.lumaOffset,
                        colorQuarterWordHue: cfgParams.colorQuarterWord.hue,
                        colorQuarterWordSat: cfgParams.colorQuarterWord.sat,
                        colorQuarterWordLumaOffset: cfgParams.colorQuarterWord.lumaOffset,
                        colorHoursNumeralHue: cfgParams.colorHoursNumeral.hue,
                        colorHoursNumeralSat: cfgParams.colorHoursNumeral.sat,
                        colorHoursNumeralLumaOffset: cfgParams.colorHoursNumeral.lumaOffset,
                        colorClockWordHue: cfgParams.colorClockWord.hue,
                        colorClockWordSat: cfgParams.colorClockWord.sat,
                        colorClockWordLumaOffset: cfgParams.colorClockWord.lumaOffset
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
                        this.showIp = response.data.showIp;
                        this.timeZoneOffset = response.data.timeZoneOffset;
                        this.dayLightSaving = response.data.dayLightSaving;
                        this.luma = response.data.luma;
                        this.colorMinutesNumeral.hue =    response.data.colorMinutesNumeralHue;
                        this.colorMinutesNumeral.sat =  response.data.colorMinutesNumeralSat;
                        this.colorMinutesNumeral.lumaOffset =   response.data.colorMinutesNumeralLumaOffset;
                        this.colorMinWord.hue =    response.data.colorMinWordHue;
                        this.colorMinWord.sat =  response.data.colorMinWordSat;
                        this.colorMinWord.lumaOffset =   response.data.colorMinWordLumaOffset;
                        this.colorPreWord.hue =    response.data.colorPreWordHue;
                        this.colorPreWord.sat =  response.data.colorPreWordSat;
                        this.colorPreWord.lumaOffset =   response.data.colorPreWordLumaOffset;
                        this.colorQuarterWord.hue =    response.data.colorQuarterWordHue;
                        this.colorQuarterWord.sat =  response.data.colorQuarterWordSat;
                        this.colorQuarterWord.lumaOffset =   response.data.colorQuarterWordLumaOffset;
                        this.colorHoursNumeral.hue =    response.data.colorHoursNumeralHue;
                        this.colorHoursNumeral.sat =  response.data.colorHoursNumeralSat;
                        this.colorHoursNumeral.lumaOffset =   response.data.colorHoursNumeralLumaOffset;
                        this.colorClockWord.hue =    response.data.colorClockWordHue;
                        this.colorClockWord.sat =  response.data.colorClockWordSat;
                        this.colorClockWord.lumaOffset =   response.data.colorClockWordLumaOffset;
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
              showIp: cfgParams.showIp,
              timeZoneOffset: cfgParams.timeZoneOffset,
              dayLightSaving: cfgParams.dayLightSaving,
              luma: cfgParams.luma,
              colorMinutesNumeralHue: cfgParams.colorMinutesNumeral.hue,
              colorMinutesNumeralSat: cfgParams.colorMinutesNumeral.sat,
              colorMinutesNumeralLumaOffset: cfgParams.colorMinutesNumeral.lumaOffset,
              colorMinWordHue: cfgParams.colorMinWord.hue,
              colorMinWordSat: cfgParams.colorMinWord.sat,
              colorMinWordLumaOffset: cfgParams.colorMinWord.lumaOffset,
              colorPreWordHue: cfgParams.colorPreWord.hue,
              colorPreWordSat: cfgParams.colorPreWord.sat,
              colorPreWordLumaOffset: cfgParams.colorPreWord.lumaOffset,
              colorQuarterWordHue: cfgParams.colorQuarterWord.hue,
              colorQuarterWordSat: cfgParams.colorQuarterWord.sat,
              colorQuarterWordLumaOffset: cfgParams.colorQuarterWord.lumaOffset,
              colorHoursNumeralHue: cfgParams.colorHoursNumeral.hue,
              colorHoursNumeralSat: cfgParams.colorHoursNumeral.sat,
              colorHoursNumeralLumaOffset: cfgParams.colorHoursNumeral.lumaOffset,
              colorClockWordHue: cfgParams.colorClockWord.hue,
              colorClockWordSat: cfgParams.colorClockWord.sat,
              colorClockWordLumaOffset: cfgParams.colorClockWord.lumaOffset,
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