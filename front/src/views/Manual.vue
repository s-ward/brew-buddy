<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <!-- <v-responsive :style="{ background: `rgb(${red}, ${green}, ${blue})` }" height="300px"></v-responsive> -->
          <v-card-text>
            <v-container fluid grid-list-lg>
              <v-layout row wrap>
                <v-flex xs9>
                  <h1> Manual Control</h1>
                </v-flex>
                <v-flex xs3>
                  <v-icon large color="orange darker-2" @click="gohome">home</v-icon>
                </v-flex>
                 <v-flex xs9>
                  <v-select :items="items" filled label="Mode"></v-select>
                </v-flex>
                <v-flex xs9>
                  <v-slider v-model="heaterpower" :max="100" :min="1" label="Heater Power"></v-slider>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="heaterpower" class="mt-0" type="number" hint="%" persistent-hint></v-text-field>
                </v-flex>
                <v-flex xs9>
                  <v-slider v-model="targettemp" :max="110" label="Target Temp"></v-slider>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="targettemp" class="mt-0" type="number" hint="°C" persistent-hint></v-text-field>
                </v-flex>
                <v-flex xs9>
                  <v-slider v-model="targetflow" :max="110" label="Target Flow"></v-slider>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="targetflow" class="mt-0" type="number" hint="mL/m" persistent-hint></v-text-field>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="currenttemp" label = "Current Temp" class="mt-0" type="number" value="0" readonly outlined></v-text-field>
                </v-flex>
                <v-flex xs9>
                  <chart></chart>
                </v-flex>
                <v-flex xs6>
                  <v-switch v-model="heater" @change="set_color" true-value="On" false-value="Off" :label="`Heater: ${heater.toString()}`"></v-switch>
                </v-flex>
                <v-flex xs6>
                  <v-switch v-model="pump" @change="set_color" true-value="On" false-value="Off" :label="`Pump: ${pump.toString()}`"></v-switch>
                </v-flex>
                <v-flex xs9>
                  <v-switch v-model="valve1" true-value="Open" false-value="Closed" :label="`Valve 1: ${valve1.toString()}`"></v-switch>
                </v-flex>
                <v-flex xs9>
                  <v-switch v-model="valve2" true-value="Open" false-value="Closed" :label="`Valve 2: ${valve2.toString()}`"></v-switch>
                </v-flex>
                <v-flex xs9>
                  <v-switch v-model="valve3" true-value="Open" false-value="Closed" :label="`Valve 3: ${valve3.toString()}`"></v-switch>
                </v-flex>
              </v-layout>
            </v-container>
          </v-card-text>
          <v-btn fab dark large color="red accent-4" @click="set_color">
            <v-icon dark>check_box</v-icon>
          </v-btn>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
import Chart from './Chart.vue'

export default {
  data () {
    return {
      targettemp: 0,
      valve1: 'Closed',
      valve2: 'Closed',
      valve3: 'Closed',
      heater: 'Off',
      pump: 'Off',
      heaterpower: 0,
      items: ['None', 'Temperature Control', 'Flow Control']
    }
  },
  methods: {
    set_color: function () {
      this.$ajax
        .post('/api/v1/manual/brightness', {
          targettemp: this.targettemp,
          heater: this.heater,
          pump: this.pump,
          valve1: this.valve1,
          valve2: this.valve2,
          valve3: this.valve3
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
    },
    gohome () {
      this.$router.push('/')
    }
  },
  components: {
    Chart
  }
}
</script>
