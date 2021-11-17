<template>
<v-card height="100%" class="fill-height justify-center text-center" color="orange lighten-3" elevation="0">
  <v-card-title>
    <span class="text-h5 white--text">Brewery Setup</span>
    <v-spacer></v-spacer>
    <v-btn @click="gohome" icon><v-icon>home</v-icon></v-btn>
  </v-card-title>
  <hr style="height:2px; width:90%; border-width:0; color:yellow; margin:5%; margin-top:0%; background-color:orange">

  <v-card-text>
    <v-row>
      <v-col>
        <v-text-field v-model.number="kettlevolume" :max="100" :min="0" outlined hide-details label="Kettle Volume" class="mt-0" type="number" suffix="L" persistent-hint></v-text-field>
      </v-col>
      <v-col>
        <v-text-field v-model.number="mashtunvolume" :max="100" :min="0" outlined hide-details label="Mash Tun Volume" class="mt-0" type="number" suffix="L" persistent-hint></v-text-field>
      </v-col>
    </v-row>
    <v-row>
        <v-col cols="12">
          <v-checkbox
            v-model="pumpedtransfer"
            label="Pumped Transfer"
            :on-icon="'check_circle_outline'"
            :off-icon="'highlight_off'"
            color="green"
            hide-details
          ></v-checkbox>
        </v-col>
    </v-row>

    <v-row>
      <v-col>
        <v-select v-model="heatingmethod" :items="heatingmethods" outlined hide-details label="Heating Method"></v-select>
      </v-col>
      <v-col>
        <v-select v-model="coolingmethod" :items="coolingmethods" outlined hide-details label="Cooling Method"></v-select>
      </v-col>
    </v-row>
        <v-card-subtitle>
          <span class="text-h6 white--text">General Settings</span>
        </v-card-subtitle>
        <v-row>
          <v-col>
            <v-switch v-model="units" true-value="Imperial" false-value="Metric" :label="`Units: ${units.toString()}`"></v-switch>
          </v-col>
          <v-col cols="10">
            <v-text-field v-model.number="leadtime" :max="20" :min="1" label="Lead Time for User Interaction" class="mt-0" type="number" suffix="min" ></v-text-field>
          </v-col>
        </v-row>
        <v-row align="center" justify="space-around">
          <v-btn @click="save_setup">Save</v-btn>
          <v-btn @click="goback">Cancel</v-btn>
        </v-row>

  </v-card-text>
  </v-card>
</template>

<script>

export default {
  props: {
    id: {
      type: String,
      default: 'fromMain'
    }
  },
  data () {
    return {
      kettlevolume: 0,
      mashtunvolume: 0,
      pumpedtransfer: true,
      heatingmethod: '',
      coolingmethod: '',
      units: '',
      leadtime: 0,
      heatingmethods: ['RIMS', 'Element'],
      ex4: ['red', 'indigo', 'orange', 'primary', 'secondary', 'success', 'info', 'warning', 'error', 'red darken-3', 'indigo darken-3', 'orange darken-3'],
      coolingmethods: ['None', 'Wort Recirc', 'Water Recirc'] }
  },
  mounted () {
    this.$ajax
      .get('/api/v1/setup/load')
      .then(data => {
        this.kettlevolume = data.data.kettlevolume
        this.mashtunvolume = data.data.mashtunvolume
        this.pumpedtransfer = data.data.pumpedtransfer
        this.heatingmethod = data.data.heatingmethod
        this.coolingmethod = data.data.coolingmethod
        this.units = data.data.units
        this.leadtime = data.data.leadtime
      })
      .catch(error => {
        console.log(error)
      })
  },
  methods: {
    goback () {
      if (this.id === 'fromBrew') {
        this.$router.push('/brew')
      } else {
        this.$router.push('/')
      }
    },
    gohome () {
      this.$router.push('/')
    },
    save_setup: function () {
      this.$ajax
        .post('/api/v1/setup/save', {
          kettlevolume: this.kettlevolume,
          mashtunvolume: this.mashtunvolume,
          pumpedtransfer: this.pumpedtransfer,
          heatingmethod: this.heatingmethod,
          coolingmethod: this.coolingmethod,
          units: this.units,
          leadtime: this.leadtime
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
        .then(this.goback)
    }
  }
}
</script>
