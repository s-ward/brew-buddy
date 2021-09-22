<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-card-text>
            <v-container fluid grid-list-lg>
              <v-layout row wrap>
                <v-flex xs9>
                  <h1> Brew</h1>
                </v-flex>
                <v-flex xs3>
                  <v-icon large color="orange darker-2" @click="gohome">home</v-icon>
                </v-flex>
                <v-flex xs9>
                  <h2>Brewery Setup</h2>
                </v-flex>
                <v-flex xs3>
                  <v-icon small color="orange darker-2" @click="gosetup">settings</v-icon>
                </v-flex>
                <v-flex xs6>
                  <v-text-field v-model.number="kettlevolume" readonly :max="100" :min="0" label="Kettle Volume" class="mt-0" type="number" hint="L" persistent-hint></v-text-field>
                </v-flex>
                <v-flex xs6>
                  <v-text-field v-model.number="mashtunvolume" disabled :max="100" :min="0" label="Mash Tun Volume" class="mt-0" type="number" hint="L" persistent-hint></v-text-field>
                </v-flex>
                <v-flex xs6>
                  <v-checkbox v-model="pumpedtransfer" disabled label="Pumped Transfer"></v-checkbox>
                </v-flex>
                <v-flex xs9>
                  <v-select v-model="heatingmethod" disabled :items="heatingmethods" filled label="Heating Method"></v-select>
                </v-flex>
                <v-flex xs9>
                  <v-select v-model="coolingmethod" disabled :items="coolingmethods" filled label="Cooling Method"></v-select>
                </v-flex>
                <v-flex xs9>
                  <h2>Recipe</h2>
                </v-flex>
                <v-flex xs3>
                  <v-icon small color="orange darker-2" @click="gobook">book</v-icon>
                </v-flex>
                <v-flex xs9>

                </v-flex>
                <v-flex xs6>

                </v-flex>
              </v-layout>
            </v-container>
          </v-card-text>
          <v-btn @click="save_setup">Save</v-btn>
          <v-btn @click="gohome">Cancel</v-btn>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>

export default {
  data () {
    return {
      kettlevolume: 0,
      mashtunvolume: 0,
      pumpedtransfer: false,
      heatingmethod: '',
      coolingmethod: '',
      heatingmethods: ['None', 'RIMS', 'Element'],
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
    gohome () {
      this.$router.push('/')
    },
    gobook () {
      this.$router.push('/book')
    },
    gosetup () {
      this.$router.push('/setup')
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
    }
  }
}
</script>
