<template>
  <v-card height="100%" class="fill-height justify-center text-center" color="orange lighten-3" elevation="0">
    <v-card-title>
      <span class="text-h5 white--text">Manual Control</span>
      <v-spacer></v-spacer>
      <v-btn @click="gohome" color="orange accent-3" icon><v-icon large>home</v-icon></v-btn>
    </v-card-title>
    <hr style="height:2px; width:90%; border-width:0; color:orange; margin:5%; margin-top:0%; background-color:orange">

    <v-card-text>
      <v-row>
        <v-col>
          <v-select v-model="mode" @change="set_manual" :items="modes" dense hide-details outlined label="Mode" align-center class="align-center"></v-select>
        </v-col>
      </v-row>
      <!-- Option 1 with thumb labels -->
      <!-- <v-row>
        <v-col>
          <v-sheet outlined color="rgba(0,0,0,0.2)" rounded>
            <v-card color="orange lighten-3" outlined elevation="0">
              <v-card-text>
                <v-row>
                  <v-col class="py-0 my-0">
                    <v-subheader class="pl-0">
                      Heater Power %
                    </v-subheader>
                    <v-slider
                    v-model="heaterpower"
                    hide-details
                    :disabled="mode === 'Temperature Control'"
                    thumb-label="always"
                    :thumb-size="18"
                    color="orange accent-3"
                    track-color="disabled"
                    :min="0"
                    :max="100"
                    class="my-0 py-0"
                    >
                      <template v-slot:prepend>
                        <v-icon @click="minusheaterpower" color="orange accent-3" :disabled="mode === 'Temperature Control'">remove</v-icon>
                      </template>
                      <template v-slot:append>
                        <v-icon @click="addheaterpower" color="orange accent-3" :disabled="mode === 'Temperature Control'">add</v-icon>
                      </template>
                    </v-slider>
                  </v-col>
                </v-row>

                <v-row>
                  <v-col class="py-0 my-0">
                    <v-subheader class="pl-0">
                      Target Temp °C
                    </v-subheader>
                    <v-slider
                    v-model="targettemp"
                    hide-details
                    :disabled="mode === 'None'"
                    thumb-label="always"
                    :thumb-size="18"
                    color="orange accent-3"
                    track-color="disabled"
                    :min="0"
                    :max="110"
                    class="my-0 py-0"
                    >
                      <template v-slot:prepend>
                        <v-icon @click="minustargettemp" color="orange accent-3" :disabled="mode === 'None'">remove</v-icon>
                      </template>
                      <template v-slot:append>
                        <v-icon @click="addtargettemp" color="orange accent-3" :disabled="mode === 'None'">add</v-icon>
                      </template>
                    </v-slider>
                  </v-col>
                </v-row>

                <v-row>
                  <v-col class="py-0 my-0">
                    <v-subheader class="pl-0">
                      Flow Rate %
                    </v-subheader>
                    <v-slider
                    v-model="targetflow"
                    hide-details
                    thumb-label="always"
                    :thumb-size="18"
                    color="orange accent-3"
                    track-color="disabled"
                    :min="0"
                    :max="100"
                    class="my-0 py-0"
                    >
                      <template v-slot:prepend>
                        <v-icon @click="minustargetflow" color="orange accent-3">remove</v-icon>
                      </template>
                      <template v-slot:append>
                        <v-icon @click="addtargetflow" color="orange accent-3">add</v-icon>
                      </template>
                    </v-slider>
                  </v-col>
                </v-row>
                <v-container>
                  <v-row align="center" justify="space-around">
                    <v-col>
                      <v-btn @click="editvalues">Edit</v-btn>
                    </v-col>
                    <v-col>
                      <v-btn @click="updatevalues">Update</v-btn>
                    </v-col>
                  </v-row>
                </v-container>
              </v-card-text>
            </v-card>
          </v-sheet>
        </v-col>
      </v-row> -->
      <!-- End of option 1 with thumb labels -->

      <!-- Option 2 with temporary thumb labels and label value-->
      <!-- <v-row>
        <v-col>
          <v-sheet outlined color="rgba(0,0,0,0.2)" rounded>
            <v-card color="orange lighten-3" outlined elevation="0">
              <v-card-text>
                <v-row>
                  <v-col class="py-0 my-0">
                    <v-subheader class="pl-0">
                      Heater Power: <b>{{this.heaterpower}}%</b>
                    </v-subheader>
                    <v-slider
                    v-model="heaterpower"
                    hide-details
                    :disabled="mode === 'Temperature Control'"
                    thumb-label
                    :thumb-size="18"
                    color="orange accent-3"
                    track-color="disabled"
                    :min="0"
                    :max="100"
                    class="my-0 py-0"
                    >
                      <template v-slot:prepend>
                        <v-icon @click="minusheaterpower" color="orange accent-3" :disabled="mode === 'Temperature Control'">remove</v-icon>
                      </template>
                      <template v-slot:append>
                        <v-icon @click="addheaterpower" color="orange accent-3" :disabled="mode === 'Temperature Control'">add</v-icon>
                      </template>
                    </v-slider>
                  </v-col>
                </v-row>

                <v-row>
                  <v-col class="py-0 my-0">
                    <v-subheader class="pl-0">
                      Target Temp: <b>{{this.targettemp}}°C</b>
                    </v-subheader>
                    <v-slider
                    v-model="targettemp"
                    hide-details
                    :disabled="mode === 'None'"
                    thumb-label
                    :thumb-size="18"
                    color="orange accent-3"
                    track-color="disabled"
                    :min="0"
                    :max="110"
                    class="my-0 py-0"
                    >
                      <template v-slot:prepend>
                        <v-icon @click="minustargettemp" color="orange accent-3" :disabled="mode === 'None'">remove</v-icon>
                      </template>
                      <template v-slot:append>
                        <v-icon @click="addtargettemp" color="orange accent-3" :disabled="mode === 'None'">add</v-icon>
                      </template>
                    </v-slider>
                  </v-col>
                </v-row>

                <v-row>
                  <v-col class="py-0 my-0">
                    <v-subheader class="pl-0">
                      Flow Rate: <b>{{this.targetflow}}%</b>
                    </v-subheader>
                    <v-slider
                    v-model="targetflow"
                    hide-details
                    thumb-label
                    :thumb-size="18"
                    color="orange accent-3"
                    track-color="disabled"
                    :min="0"
                    :max="100"
                    class="my-0 py-0"
                    >
                      <template v-slot:prepend>
                        <v-icon @click="minustargetflow" color="orange accent-3">remove</v-icon>
                      </template>
                      <template v-slot:append>
                        <v-icon @click="addtargetflow" color="orange accent-3">add</v-icon>
                      </template>
                    </v-slider>
                  </v-col>
                </v-row>
                <v-container>
                  <v-row align="center" justify="space-around">
                    <v-col>
                      <v-btn @click="editvalues">Edit</v-btn>
                    </v-col>
                    <v-col>
                      <v-btn @click="updatevalues">Update</v-btn>
                    </v-col>
                  </v-row>
                </v-container>
              </v-card-text>
            </v-card>
          </v-sheet>
        </v-col>
      </v-row> -->
      <!-- End of option 2 with temporary thumb labels and label values -->

      <!-- Option 3 without thumb labels -->
      <v-row>
        <v-col>
          <v-sheet outlined color="rgba(0,0,0,0.2)" rounded>
            <v-card color="orange lighten-3" outlined elevation="0">
              <v-card-text>
                <v-row>
                  <v-col class="py-0 my-0">
                    <v-subheader class="pa-0 ma-0">
                      Heater Power: <b>{{this.compheaterpowertextmodel}}</b>
                    </v-subheader>
                    <v-slider
                    v-model="compheaterpowerslidermodel"
                    @change="set_manual"
                    hide-details
                    :disabled="mode === 'Temperature Control'"
                    color="orange accent-3"
                    track-color="disabled"
                    :min="0"
                    :max="100"
                    class="my-0 py-0"
                    >
                      <template v-slot:prepend>
                        <v-icon @click="minusheaterpower(); set_manual();" class="py-0 my-0" color="orange accent-3" :disabled="mode === 'Temperature Control'">remove_circle_outline</v-icon>
                      </template>
                      <template v-slot:append>
                        <v-icon @click="addheaterpower(); set_manual();" class="py-0 my-0" color="orange accent-3" :disabled="mode === 'Temperature Control'">add_circle_outline</v-icon>
                      </template>
                    </v-slider>
                  </v-col>
                </v-row>

                <v-row>
                  <v-col class="py-0 my-0">
                    <v-subheader class="pa-0 ma-0">
                      Target Temp: <b>{{this.comptargettemptextmodel}}</b>
                    </v-subheader>
                    <v-slider
                    v-model="comptargettempslidermodel"
                    @change="set_manual"
                    hide-details
                    :disabled="mode === 'None'"
                    color="orange accent-3"
                    track-color="disabled"
                    :min="0"
                    :max="110"
                    class="my-0 py-0"
                    >
                      <template v-slot:prepend>
                        <v-icon @click="minustargettemp(); set_manual();" class="py-0 my-0" color="orange accent-3" :disabled="mode === 'None'">remove_circle_outline</v-icon>
                      </template>
                      <template v-slot:append>
                        <v-icon @click="addtargettemp(); set_manual();" class="py-0 my-0" color="orange accent-3" :disabled="mode === 'None'">add_circle_outline</v-icon>
                      </template>
                    </v-slider>
                  </v-col>
                </v-row>

                <v-row>
                  <v-col class="py-0 my-0">
                    <v-subheader class="pa-0 ma-0">
                      Flow Rate: <b>{{this.targetflow}}%</b>
                    </v-subheader>
                    <v-slider
                    v-model="targetflow"
                    @change="set_manual"
                    hide-details
                    color="orange accent-3"
                    track-color="disabled"
                    :min="0"
                    :max="100"
                    class="my-0 py-0"
                    >
                      <template v-slot:prepend>
                        <v-icon @click="minustargetflow(); set_manual();" class="py-0 my-0" color="orange accent-3">remove_circle_outline</v-icon>
                      </template>
                      <template v-slot:append>
                        <v-icon @click="addtargetflow(); set_manual();" class="py-0 my-0" color="orange accent-3">add_circle_outline</v-icon>
                      </template>
                    </v-slider>
                  </v-col>
                </v-row>

                <!-- <v-container>
                  <v-row align="center" justify="space-around">
                    <v-col>
                      <v-btn @click="editvalues">Edit</v-btn>
                    </v-col>
                    <v-col>
                      <v-btn @click="updatevalues">Update</v-btn>
                    </v-col>
                  </v-row>
                </v-container> -->

              </v-card-text>
            </v-card>
          </v-sheet>
        </v-col>
      </v-row>
      <!-- End of option 3 without thumb labels -->

      <v-row>
        <v-col cols="6">
          <v-text-field :value="get_chart_value" label="Temp" suffix="°C" class="mt-0" type="number" hide-details readonly outlined></v-text-field>
          <chart></chart>
        </v-col>
        <v-col cols="6">
          <v-text-field :value="get_flow_chart_value" label="Flow" suffix="mL/m" class="mt-0" type="number" hide-details readonly outlined></v-text-field>
          <chart></chart>
        </v-col>
      </v-row>

      <v-row align="center" align-content="center" justify="center">
        <v-col cols="6" class="mb-0 pb-0">
          <v-switch v-model="comppumpswitchmodel" @change="set_manual" true-value="On" false-value="Off" class="mt-0 pt-0" inset dense :label="`Pump: ${pump.toString()}`"></v-switch>
         </v-col>
        <v-col cols="6" class="mb-0 pb-0">
          <v-switch v-model="heater" @change="set_manual" true-value="On" false-value="Off" :disabled="pump === 'Off'" class="mt-0 pt-0" inset dense :label="`Heater: ${heater.toString()}`"></v-switch>
        </v-col>
      </v-row>

      <v-row align="center">
        <v-col cols="12" align-self="center" class="mt-0 pt-0">
          <v-switch v-model="valve1" @change="set_manual" true-value="External" false-value="Internal" class="mt-0 pt-0" inset dense :label="`Valve Tap In: ${valve1.toString()}`"></v-switch>
        <!-- </v-col>
        <v-col cols="12"> -->
          <v-switch v-model="valve2" @change="set_manual" true-value="External" false-value="Internal" class="mt-0 pt-0" inset dense :label="`Valve Sparge In: ${valve2.toString()}`"></v-switch>
        <!-- </v-col>
        <v-col cols="12"> -->
          <v-switch v-model="valve3" @change="set_manual" true-value="External" false-value="Internal" class="mt-0 pt-0" inset dense :label="`Valve Sparge Out: ${valve3.toString()}`"></v-switch>
        </v-col>
      </v-row>

    </v-card-text>
  </v-card>
</template>

<script>
import Chart from './Chart.vue'

export default {
  data () {
    return {
      mode: 'None',
      heaterpower: 50,
      targettemp: 0,
      targetflow: 50,
      heater: 'Off',
      pump: 'Off',
      valve1: 'Internal',
      valve2: 'Internal',
      valve3: 'Internal',
      editsliders: false,
      modes: ['None', 'Temperature Control']
    }
  },
  methods: {
    set_manual: function () {
      this.$ajax
        .post('/api/v1/manual/set', {
          mode: this.mode,
          heaterpower: this.heaterpower,
          targettemp: this.targettemp,
          targetflow: this.targetflow,
          pump: (this.pump === 'On'),
          heater: (this.heater === 'On'),
          valve1: (this.valve1 === 'Internal'),
          valve2: (this.valve2 === 'Internal'),
          valve3: (this.valve3 === 'Internal')
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
    },
    // gohome () {
    //   // go back to passive state
    //   this.$router.push('/')
    // },
    gohome1: function () {
      this.$ajax
        .post('/api/v1/changestate', {
          brewstate: 'passive'
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
        .then(this.$router.push('/'))
    },
    gohome: function () {
      // this.$store.dispatch('set_brew_state_action', {
      this.$store.dispatch('post_state_update', {
        // brewstate: this.$store.state.brewstate,
        brewint: this.$store.state.brewint,
        pauseint: this.$store.state.pauseint,
        cancelint: 1,
        cleanint: this.$store.state.cleanint,
        userintreq: this.$store.state.userintreq,
        adjunctreq: this.$store.state.adjunctreq

      }) // then get/update store states
        // .then(() => this.$store.dispatch('post_state_update')
        //.then(() => this.$store.dispatch('get_brew_state_action')
          .then(() => (this.$router.push('/')))
    },
    minustargetflow () {
      this.targetflow--
    },
    addtargetflow () {
      this.targetflow++
    },
    minustargettemp () {
      this.targettemp--
    },
    addtargettemp () {
      this.targettemp++
    },
    minusheaterpower () {
      this.heaterpower--
    },
    addheaterpower () {
      this.heaterpower++
    },
    editvalues () {

    },
    updateData: function () {
      this.$store.dispatch('update_temp1'),
      this.$store.dispatch('update_flow1')
    }
  },

  components: {
    Chart
  },
  computed: {
    get_chart_value () {
      return this.$store.state.temp1_value
    },
    get_flow_chart_value () {
      return this.$store.state.flow1_value
    },
    compheaterpowertextmodel () {
      if (this.mode === 'None') {
        return this.heaterpower + '%'
      } else {
        return 'Auto'
      }
    },
    comptargettemptextmodel () {
      if (this.mode === 'Temperature Control') {
        return this.targettemp + '°C'
      } else {
        return 'Auto'
      }
    },
    compheaterpowerslidermodel: {
      get () {
        if (this.mode === 'None') {
          return this.heaterpower
        } else {
          return 0
        }
      },
      set (val) {
        if (this.mode === 'None') {
          this.heaterpower = val
        }
      }
    },
    comptargettempslidermodel: {
      get () {
        if (this.mode === 'Temperature Control') {
          return this.targettemp
        } else {
          return 0
        }
      },
      set (val) {
        if (this.mode === 'Temperature Control') {
          this.targettemp = val
        }
      }
    },
    compheaterswitchmodel: {
      get () {
        if (this.pump === 'On') {
          return this.heater
        } else {
          return 'Off'
        }
      },
      set (val) {
        if (this.pump === 'Off') {
          this.heater = 'Off'
        } else {
          this.heater = val
        }
      }
    },
    // switches heater off as well if pump is switched off
    comppumpswitchmodel: {
      get () {
        return this.pump
      },
      set (val) {
        this.pump = val
        if (val === 'Off') {
          this.heater = val
        }
      }
    }
  }
  // },
  // mounted () {
  //   clearInterval(this.timer);
  //   this.timer = setInterval(this.updateData, 5000);
  // },
  // destroyed: function () {
  //   clearInterval(this.timer);
  // }
}
</script>
