<template>
<v-card height="100%" class="fill-height justify-center text-center" color="orange lighten-3" elevation="0">
  <v-card-title class="text-h5 white--text">
    Brew Progress
    <v-spacer></v-spacer>
    <v-btn @click="gohome" color="orange accent-3" icon><v-icon large>home</v-icon></v-btn>
  </v-card-title>
  <hr style="height:2px; width:90%; border-width:0; color:yellow; margin:5%; margin-top:0%; background-color:orange">
  <v-card-text>
    <v-row align="center">
      <v-col cols="8">
        <v-text-field
        :value="get_current_stage"
        hide-details
        readonly
        outlined
        dense
        label="Current Stage"
        class="mt-0 align-center"
        ></v-text-field>
      </v-col>
      <v-col cols="4">
        <div class="pa-1 white rounded-circle d-inline-block">
          <v-icon :color="statusIconColor" class="pa-0 ma-0" size="50">{{statusIcon}}</v-icon>
        </div>
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <v-text-field
        :value="get_current_step"
        hide-details
        readonly
        outlined
        dense
        label="Current Step"
        class="mt-0 align-center"
        ></v-text-field>
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <v-sheet outlined color="rgba(0,0,0,0.2)" rounded>
          <v-card color="orange lighten-3" outlined elevation="0" class="justify-left text-left">
            <v-card-text id="textarea" v-model="textarea" class="pa-3 overflow-y-auto" :style="text_area_style" overflow-y-scroll>
              <div v-for="item in get_text_area" :key="item" :class="item.textcolor">{{item.time}} {{item.text}}</div>
            </v-card-text>
            <v-card-actions>
              <v-spacer></v-spacer>
              <v-btn v-if="get_adjunct_req || get_interaction_req" small @click="confirm">confirm</v-btn>
              <v-spacer></v-spacer>
            </v-card-actions>
          </v-card>
        </v-sheet>
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <v-text-field
          :value="get_target_temp"
          readonly
          outlined
          dense
          hide-details
          label="Target Temp"
          class="mt-0"
          suffix="°C"
        ></v-text-field>
      </v-col>
      <v-col>
        <v-text-field
        :value="get_time_remaining"
        readonly
        outlined
        dense
        hide-details
        label="Time Remaining"
        class="mt-0"
        ></v-text-field>
      </v-col>
    </v-row>
    <v-row>
      <v-col cols="6">
        <v-text-field
        :value="get_chart_value"
        readonly
        outlined
        dense
        hide-details
        label="Temperature"
        class="mt-0"
        suffix="°C"
        type="number"
        ></v-text-field>
        <chart></chart>
      </v-col>
      <v-col>
        <v-text-field
          :value="get_flow_chart_value"
          readonly
          outlined
          dense
          hide-details
          label="Flow Rate"
          class="mt-0"
          suffix="mL/m"
        ></v-text-field>
        </v-col>
    </v-row>
    <v-row align="center" justify="space-around">
      <v-col>
        <v-btn v-if="get_brew_status !==1" :disabled="get_interaction_req" @click="pause">Pause</v-btn>
        <v-btn v-if="get_brew_status ===1" :disabled="get_interaction_req" @click="resume">Resume</v-btn>
      </v-col>
      <v-col>
        <v-btn @click="cancel">Cancel</v-btn>
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
      currentstage: '',
      // status: '',
      statusnotes: '',
      currenttemp: 0,
      targettemp: 0,
      timereamining: 0,
      flowrate: 0,
      timer: null,
      textarea: ''

    }
  },

  methods: {
    gohome () {
      this.$router.push('/')
    },
    pause2 () {
      console.log('Pause 2 button pressed' + this.$store.state.userintreq)
    },
    pause () {
      // this.$store.dispatch('pause')
      // this.$store.commit('set_brew_status', 2)
      console.log('Brew Int' + this.$store.state.brewint)
      this.$store.dispatch('post_state_update', {
        brewint: this.$store.state.brewint,
        pauseint: 1,
        cancelint: this.$store.state.cancelint,
        cleanint: this.$store.state.cleanint,
        userintreq: this.$store.state.userintreq,
        adjunctreq: this.$store.state.adjunctreq
      })
        .then(() => (
          this.$store.commit('add_message', {
            textcolor: 'yellow--text',
            time: this.get_time_remaining,
            text: 'Brew Paused'
          })
        ))
    },
    cancel () {
      // add confirmation
      console.log('Brew Int' + this.$store.state.brewint)
      // this.$store.dispatch('set_brew_state_action', {
      this.$store.dispatch('post_state_update', {
        brewint: this.$store.state.brewint,
        pauseint: this.$store.state.pauseint,
        cancelint: 1,
        cleanint: this.$store.state.cleanint,
        userintreq: this.$store.state.userintreq,
        adjunctreq: this.$store.state.adjunctreq
        // brewstate: this.$store.state.pauseint,
        // pauseint: this.$store.state.pauseint,
        // cancelint: 1
      })
        .then(() => (
          // this.$store.commit('add_message', {
          //   textcolor: 'red--text',
          //   time: this.get_time_remaining,
          //   text: 'Brew Cancelled'
          // })
          this.$store.commit('clear_messages')
        ))
        // .then(() => this.$store.dispatch('post_state_update')
        .then(() => this.$router.push('/'))
    },
    resume () {
      // this.$store.commit('set_brew_status', 1)
      this.$store.dispatch('post_state_update', {
        brewint: this.$store.state.brewint,
        pauseint: 0,
        cancelint: this.$store.state.cancelint,
        cleanint: this.$store.state.cleanint,
        userintreq: this.$store.state.userintreq,
        adjunctreq: this.$store.state.adjunctreq
      })
        .then(() => (
          this.$store.commit('add_message', {
            textcolor: 'green--text',
            time: this.get_time_remaining,
            text: 'Brew Resumed'
          })
        ))
    },
    confirm () {
      // if user int req - confirm sends config message else if adj req send adj message else disabled
      // if (this.$store.userintreq) {
      if (this.get_interaction_req) {
        // add message to stack
        // this.$store.commit('add_message', {
        this.$store.dispatch('add_message_action', {
          textcolor: 'green--text',
          time: this.get_time_remaining,
          text: 'Brew Resumed: Configuration Confirmed'
        })
        // unpause
          .then(() => (
            // this.resume()
            this.$store.dispatch('post_state_update', {
              brewint: this.$store.state.brewint,
              pauseint: 0,
              cancelint: this.$store.state.cancelint,
              cleanint: this.$store.state.cleanint,
              userintreq: this.$store.state.userintreq,
              adjunctreq: this.$store.state.adjunctreq
            })
          ))
        // set to 0
        // tell esp of unpause and set to 0
      // } else if (this.$store.adjunctreq) {
      } else if (this.get_adjunct_req) {
        // add message to stack
        this.$store.dispatch('add_message_action', {
          textcolor: 'green--text',
          time: this.get_time_remaining,
          text: 'Adjunct Addition Confirmed'
        })
        // set to 0
        // tell esp of set to 0
          .then(() => (
            this.$store.dispatch('post_state_update', {
              brewint: this.$store.state.brewint,
              pauseint: this.$store.state.pauseint,
              cancelint: this.$store.state.cancelint,
              cleanint: this.$store.state.cleanint,
              userintreq: this.$store.state.userintreq,
              adjunctreq: 0
            })
          ))
      }
    },
    updateData: function () {
      this.$store.dispatch('update_brew_progress_action')
    },
    pause_state: function () {
      this.$ajax
        .post('/api/v1/progress/state', {
          state: 2 // pause
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
    },
    // need to add confirmation
    cancel_state: function () {
      this.$ajax
        .post('/api/v1/progress/state', {
          state: 3 // cancel
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
    },
    scrollBottom: function () {
      var textarea = this.$el.querySelector('#textarea')
      textarea.scrollTop = textarea.scrollHeight
      // this.$el.scrollTop = this.$el.scrollHeight
      // document.getElementById('textarea').scrollTop = document.getElementById('textarea').scrollHeight
    }

  },
  computed: {
    get_chart_value () {
      return this.$store.state.temp1_value
    },
    get_flow_chart_value () {
      return this.$store.state.flow1_value
    },
    get_brew_status () {
      return this.$store.state.pauseint
    },
    get_target_temp () {
      return this.$store.state.targettemp
    },
    get_current_stage () {
      return this.$store.state.stage
    },
    get_status_notes () {
      return this.$store.state.step
    },
    get_minutes_remaining () {
      return this.$store.state.minutesremaining
    },
    get_seconds_remaining () {
      return this.$store.state.secondsremaining
    },
    get_time_remaining () {
      return `${this.$store.state.minutesremaining}: ${this.$store.state.secondsremaining}`
    },
    get_current_step () {
      return `${this.$store.state.step} : ${this.$store.state.autoprocess}`
    },
    statusIconColor () {
      if (this.$store.state.brewstate === 3) {
        return ''
      } else if (this.$store.state.userintreq === 1) {
        return 'red'
      } else if (this.$store.state.pauseint === 1) {
        return 'yellow'
      } else if (this.$store.state.adjunctreq === 1) {
        return 'orange'
      } else if (this.$store.state.pauseint === 0) {
        return 'green'
      } else {
        return ''
      }
    },
    statusIcon () {
      if (this.$store.state.brewstate === 3) {
        return ''
      } else if (this.$store.state.userintreq === 1) {
        return 'stop'
      } else if (this.$store.state.pauseint === 1) {
        return 'pause_circle'
      } else if (this.$store.state.adjunctreq === 1) {
        return 'error'
      } else if (this.$store.state.pauseint === 0) {
        return 'play_circle'
      } else {
        return ''
      }
    },
    statusTextColor () {
      if (this.$store.state.brewstate === 3) {
        return ''
      } else if (this.$store.state.userintreq === 1) {
        return 'red--text'
      } else if (this.$store.state.pauseint === 1) {
        return 'yellow--text'
      } else if (this.$store.state.adjunctreq === 1) {
        return 'orange--text'
      } else if (this.$store.state.pauseint === 0) {
        return 'green--text'
      } else {
        return ''
      }
    },
    get_adjunct_message () {
      return this.$store.state.adjunctreqmessage
    },
    get_adjunct_req () {
      return this.$store.state.adjunctreq
    },
    get_interaction_message () {
      return this.$store.state.userintreqmessage
    },
    get_interaction_req () {
      if (this.$store.state.userintreq) {
        return true
      } else {
        return false
      }
    },
    get_text_area () {
      // document.getElementById('textarea').scrollTop = document.getElementById('textarea').scrollHeight
      return this.$store.state.textarea2
    },
    // reduce height of text area if on mobile
    text_area_style () {
      switch (this.$vuetify.breakpoint.name) {
        case 'xs': return 'height: 100px'
      }
      return 'height: 300px'
    }
  },
  mounted () {
    clearInterval(this.timer)
    this.timer = setInterval(this.updateData, 1000) // every second get data update
    this.$nextTick(() => this.scrollBottom())
  },
  destroyed: function () {
    clearInterval(this.timer)
  },
  components: {
    Chart
  },
  watch: {
    get_text_area () {
      // console.log('updated text area')
      this.$nextTick(() => this.scrollBottom())
    }
  }

}

</script>
