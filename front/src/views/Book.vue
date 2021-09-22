<template>



  <v-container>

<v-card
    class="mx-auto"
    max-width="400"
    tile
  >
    <v-list-tile>
      <v-list-tile-content>
        <v-list-tile-title>Single-line item</v-list-tile-title>
      </v-list-tile-content>
    </v-list-tile>

    <v-list-tile two-line>
      <v-list-tile-content>
        <v-list-tile-title>Two-line item</v-list-tile-title>
        <v-list-tile-subtitle>Secondary text</v-list-tile-subtitle>
      </v-list-tile-content>
    </v-list-tile>

    <v-list-tile three-line>
      <v-list-tile-content>
        <v-list-tile-title>Three-line item</v-list-tile-title>
        <v-list-tile-subtitle>
          Secondary line text Lorem ipsum dolor sit amet,
        </v-list-tile-subtitle>
        <v-list-tile-subtitle>
          consectetur adipiscing elit.
        </v-list-tile-subtitle>
      </v-list-tile-content>
    </v-list-tile>
  </v-card>




    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-card-text>
            <v-container fluid grid-list-lg>
              <v-layout row wrap>
                <v-flex xs9>
                  <h1>Recipe Book</h1>
                  <v-header>Recipe Book2</v-header>
                </v-flex>
                <v-flex xs3>
                  <v-icon large color="orange darker-2" @click="gohome">home</v-icon>
                </v-flex>
                <v-flex xs12>
                  <v-row>
                    <v-btn icon><v-icon>add</v-icon></v-btn>
                    New REcipe
                <!--    <v-subheader>New Recipe2</v-subheader> -->
                  </v-row>
                </v-flex>
             <!--   <v-flex xs3>
                  <v-btn icon><v-icon>add</v-icon></v-btn>
                </v-flex>
                <v-flex xs9>
                  <v-subheader>New Recipe2</v-subheader>
                </v-flex> -->
               <v-flex xs12>
                          <v-card>

                  <v-list dense>
                    <v-subheader>REPORTS</v-subheader>



                      <v-list-tile-group v-model="selectedItem" color="primary">
                      <v-list-tile v-for="(item, i) in items" :key="i">
                        <v-list-tile-icon>
            <v-icon v-text="item.icon"></v-icon>
          </v-list-tile-icon>
                        <v-list-tile-content>
                          <v-list-tile-title v-text="item.text"></v-list-tile-title>
                        </v-list-tile-content>
                      </v-list-tile>
                    </v-list-tile-group>
                  </v-list>
                  </v-card>
                 </v-flex>
                <v-flex xs12>
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
      coolingmethods: ['None', 'Wort Recirc', 'Water Recirc'],
      selectedItem: 1,
      items: [
        { text: 'Real-Time', icon: 'mdi-clock' },
        { text: 'Audience', icon: 'mdi-account' },
        { text: 'Conversions', icon: 'mdi-flag' }
      ]
    }
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
    gonew () {
      this.$router.push('/recipe')
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
