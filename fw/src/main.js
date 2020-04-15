Vue.config.devtools = true;
var app = new Vue({
    el: '#wortuhrCfgMain',
    data: {
        hostnameText: 'Host name der Wortuhr',
        hostname: '',
        nightOffActiveText: 'Wortuhr in der Nacht abschalten',
        nightOffActive: true,
        nightOffTimeText: 'Ausschaltzeit',
        nightOffTime: "22:00",
        nightOnTimeText: 'Einschaltzeit',
        nightOnTime: "07:00",
        ipOfWortuhr: ""
    },
    mounted () {
        console.log("Here we are"+document.location.host)
        this.ipOfWortuhr = this.$route.params.id
        /*
        axios
          .get(window.location.host)
          .then(response => (this.info = response))
          */
      }
})