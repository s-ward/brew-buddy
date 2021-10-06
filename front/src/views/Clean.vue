<template>
  <v-card height="100%" class="fill-height justify-center text-center" color="orange lighten-3" elevation="0">
    <v-card-title class="text-h5 white--text">Clean
    <!-- <span class="text-h5 white--text">Brew</span> -->
    <v-spacer></v-spacer>
    <v-btn @click="gohome" color="orange accent-3" icon><v-icon large>home</v-icon></v-btn>
  </v-card-title>
  <hr style="height:2px; width:90%; border-width:0; color:yellow; margin:5%; margin-top:0%; background-color:orange">
  <v-card-text>
 <!-- <v-chip v-model="status" color="success">Running</v-chip> -->
    
     <v-row align="center" justify="space-around">
      <v-col>
        <v-btn large @click="fullclean">Full Clean</v-btn>
      </v-col>
    </v-row>
    
     <v-row align="center" justify="space-around">
      <v-col>
        <v-btn large @click="rinse">Rinse</v-btn>
      </v-col>
    </v-row>

  </v-card-text>

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
    pause () {
      // this.$store.dispatch('pause')
      this.$store.commit('set_brew_status', 2)
      this.$store.commit('add_message', {
        textcolor: 'yellow--text',
        time: this.get_time_remaining,
        text: 'Brew Paused'
      })
    },
    fullclean () {
      this.$store.dispatch('set_brew_state_action', { brewstate: 4 })
        .then(() => this.$store.dispatch('post_state_update')
          .then(() => this.$router.push('/progress')))
    },
    rinse () {
      this.$store.dispatch('set_brew_state_action', { brewstate: 4 })
        .then(() => this.$store.dispatch('post_state_update')
          .then(() => this.$router.push('/progress')))
    },
    resume () {
      this.$store.commit('set_brew_status', 1)
      this.$store.commit('add_message', {
        textcolor: 'green--text',
        time: this.get_time_remaining,
        text: 'Brew Resumed'
      })
    },
    confirm () {
      // if user int req - confirm sends config message else if adj req send adj message else disabled
      if (this.$store.userintreq) {
        // add message to stack
        // unpause
        // set to 0
        // tell esp of unpause and set to 0
      } else if (this.$store.adjunctreq) {
        // add message to stack
        // set to 0
        // tell esp of set to 0
      }

    },
    updateData: function () {
      this.$store.dispatch('update_brew_progress')
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
    get_brew_status () {
      // console.log(this.$store.state.status)
      return this.$store.state.status // int
      
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
      if (this.$store.state.status === 1) {
        return 'green'
      } else if (this.$store.state.status === 2) {
        return 'yellow'
      } else if (this.$store.state.status === 3) {
        return 'orange'
      } else if (this.$store.state.status === 4) {
        return 'red'
      } else {
        return ''
      }
    },
    statusIcon () {
      if (this.$store.state.status === 1) {
        return 'play_circle'
      } else if (this.$store.state.status === 2) {
        return 'pause_circle'
      } else if (this.$store.state.status === 3) {
        return 'error'
      } else if (this.$store.state.status === 4) {
        return 'stop'
      } else {
        return ''
      }
    },
    statusTextColor () {
      if (this.$store.state.status === 1) {
        return 'green--text'
      } else if (this.$store.state.status === 2) {
        return 'yellow--text'
      } else if (this.$store.state.status === 3) {
        return 'orange--text'
      } else if (this.$store.state.status === 4) {
        return 'red--text'
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
      return this.$store.state.userintreq
    },
    get_text_area () {
      // document.getElementById('textarea').scrollTop = document.getElementById('textarea').scrollHeight
      return this.$store.state.textarea2
    },
    // reduce height of text area if on mobile
    text_area_style () {
      switch (this.$vuetify.breakpoint.name) {
        case 'xs': return 'max-height: 100px'
      }
      return 'max-height: 300px'
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
  updated () {
    // console.log('updated')
    // this.$nextTick(() => this.scrollBottom())
  },
  watch: {
    get_text_area () {
      // console.log('updated text area')
      this.$nextTick(() => this.scrollBottom())
    }
    // textarea: function () {
    //   console.log('updated text area')
    //   this.$nextTick(() => this.scrollBottom())
    // }
  }

}

</script>
