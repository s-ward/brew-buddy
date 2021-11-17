<template>
  <v-card height="100%"  color="orange lighten-3" flat>
    <v-card-title>
      <v-img src="https://www.avcl.co.nz/COMP702ENEL791BrewBuddy/App/AppTest2/src/assets/logowhite.png" contain height="200"></v-img>
    </v-card-title>
    <v-card-text>
      <!-- needs get state to determine idle, running, paused, error and display icon on brew button -->
      <!-- manual button disabled if state = in progress -->
      <!-- could be achieved with spacer after update to v2.5 -->
      <v-row class="pt-6">
        <v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2">
          <v-btn :class="brewStatusTextColor" large v-if="n===2" block @click="gobrew">
            <row justify="space-around align-center">
            <b>Brew</b>
            <v-icon x-large :color="brewStatusIconColor">{{brewStatusIcon}}</v-icon>
            </row>
            </v-btn>
        </v-col>
      </v-row>
      <v-row><v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2"><v-btn  v-if="n===2" block @click="gosetup"><v-icon>settings</v-icon>Brewery Setup</v-btn></v-col></v-row>
      <v-row><v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2"><v-btn :disabled="isDisabled"  v-if="n===2" block @click="gomanual">Manual Control</v-btn></v-col></v-row>
      <v-row><v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2"><v-btn  v-if="n===2" block @click="gobook">Recipe Book</v-btn></v-col></v-row>
      <v-row><v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2"><v-btn  :disabled="isDisabled" v-if="n===2" block @click="goclean">Clean</v-btn></v-col></v-row>
    </v-card-text>
  </v-card>
</template>

<script>
export default {
  data () {
    return {
      version: null,
      cores: null
    }
  },
  mounted () {
    this.$ajax
      .get('api/v1/system/info1')
      .then(data => {
        this.version = data.data.version
        this.cores = data.data.cores
      })
      .catch(error => {
        console.log(error)
      })
    this.$store.dispatch('update_brew_progress') // sets correct status on app restart
  },
  beforeCreate () {
    // this.$store.dispatch('update_brew_progress') // sets correct brew status on app start/restart
  },
  computed: {
    iconColor () {
      return 'green'
    },
    brewStatusIconColor () {
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
    brewStatusIcon () {
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
    brewStatusTextColor () {
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
    isDisabled () {
      if (this.$store.state.brewstate !== 3) { // if status is anything other than 3
        return true
      } else {
        return false
      }
    }
  },
  methods: {
    gomanual () {
      this.$router.push('/manual')
    },
    gosetup () {
      this.$router.push({ name: 'setup', params: { id: 'fromMain' } })
    },
    gobrew () {
      // check if brew in progress
      // no brew in progress (idle state)
      if (this.$store.state.brewstate === 3) {
        this.$router.push('/brew')
      } else {
        this.$router.push('/progress')
      }
    },
    gobook () {
      this.$router.push({ name: 'book', params: { id: 'fromMain' } })
    },
    goclean () {
      this.$router.push('/clean')
    }
  }
}
</script>
