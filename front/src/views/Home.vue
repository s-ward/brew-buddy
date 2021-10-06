<template>

  <!-- <v-container fluid fill-height> -->
    <!-- <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card color="orange darken-3" text-center elevation="0">
          <v-img src="https://www.avcl.co.nz/COMP702ENEL791BrewBuddy/App/AppTest2/src/assets/logowhite.png" contain height="200"></v-img>
          <v-flex xs3></v-flex>
           <v-flex xs6> 
       <v-btn block @click="gobrew">Brew</v-btn>
         <v-btn block @click="gosetup"><v-icon>home</v-icon>Brewery Setup</v-btn>
           </v-flex>
                <v-flex xs3></v-flex>

   
        
          <v-btn @click="gomanual">Manual Control</v-btn>
          <v-btn @click="gobook">Recipe Book</v-btn>
          <v-btn @click="goclean">Clean</v-btn>
          <v-card-title primary-title>
            <div class="ma-auto">
              <span class="grey--text">IDF version: {{version}}</span>
              <br>
              <span class="grey--text">ESP cores: {{cores}}</span>
            </div>
          </v-card-title>
        </v-card>
      </v-flex>
    </v-layout> -->
     <!-- <v-main fluid fill-height class="grey lighten-3">  -->
       <!-- <v-container fluid fill-height xs12 md8 sm6> -->

     
           <!-- <v-flex xs12 sm6 fill-height> -->
  <!-- <v-row class="fill-height justify-center" justify-center :justify="center" > 
    <v-col cols="12" sm="8"  md="6" lg="4" class="justify-center" justify-center :justify="center" > -->
      <!-- <v-card class="fill-height justify-center text-center" height="100%" color="orange lighten-3" flat>   -->
         <v-card height="100%" class="fill-height justify-center text-center" color="orange lighten-3" flat>
            <!-- <v-card-title></v-card-title> -->
          <v-card-title> 
          <!-- <span class="text-h5 white--text">Manual Control</span>
        
          <v-spacer></v-spacer>
           <v-icon large color="grey" @click="gohome">home</v-icon> --> 
        <!--  <v-btn @click="gohome" icon><v-icon>home</v-icon></v-btn>           
        </v-card-title>   -->
        <!-- <v-row> 
          <v-col class="px-6"> -->
            <v-img src="https://www.avcl.co.nz/COMP702ENEL791BrewBuddy/App/AppTest2/src/assets/logowhite.png" contain height="200"></v-img>
          <!-- </v-col>
        </v-row>  -->
        </v-card-title>
        <v-card-text>
        <!-- <v-row ><v-col><v-btn block  @click="gobrew">Brew</v-btn></v-col></v-row>
        <v-row ><v-col><v-spacer><v-btn block @click="gosetup"><v-icon>settings</v-icon>Brewery Setup</v-btn></v-spacer></v-col></v-row>
        <v-row><v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2"><v-btn v-if="n===2" block  @click="gomanual">Manual Control</v-btn></v-col></v-row>
        <v-row class="px-12"><v-col><v-btn block text @click="gobook">Recipe Book</v-btn></v-col></v-row>
        <v-row class="px-12"><v-col><v-btn block @click="goclean">Clean</v-btn></v-col></v-row> -->


<!-- needs get state to determine idle, running, paused, error and display icon on brew button -->


<!-- manual button disabled if state = in progress -->
<!-- <v-chip class="ml-4 pl-4" v-model="status" color="success">Running</v-chip> -->

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
        <v-row><v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2"><v-btn v-if="n===2" block @click="gosetup"><v-icon>settings</v-icon>Brewery Setup</v-btn></v-col></v-row>
        <v-row><v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2"><v-btn :disabled="isDisabled" outlined color="white" v-if="n===2" block @click="gomanual">Manual Control</v-btn></v-col></v-row>
        <v-row><v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2"><v-btn outlined v-if="n===2" block @click="gobook">Recipe Book</v-btn></v-col></v-row>
        <v-row><v-col v-for="n in 3" :key="n" :cols="n === 2 ? 8 : 2"><v-btn :disabled="isDisabled" v-if="n===2" block @click="goclean">Clean</v-btn></v-col></v-row>
        <!-- <v-row><v-col><v-text-field :value="this.$store.state.brewstate"></v-text-field></v-col></v-row> -->
   

      </v-card-text>
         </v-card>
    <!-- </v-col>
  </v-row> -->
</template>

<script>
export default {
  data () {
    return {
      version: null,
      cores: null,

      // status info
      // statusicon1: this.$store.state.statusinfo.statusicon1,
      // statusiconcolor1: this.$store.state.statusinfo.statusiconcolor1,
      // statustextcolor1: this.$store.state.statusinfo.statustextcolor1,
  

      



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
      if (this.$store.state.status === 1) {
        return 'green'
      } else if (this.$store.state.status === 2) {
        return 'lavender'
      } else if (this.$store.state.status === 3) {
        return 'orange'
      } else if (this.$store.state.status === 4) {
        return 'red'
      } else {
        return ''
      }
    },
    brewStatusIcon () {
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
    brewStatusTextColor () {
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
    isDisabled () {
      if (this.$store.state.status) { // if status is anything other than 0
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
      // this.$router.push({ name: 'brew', params: { recipe: '' } })
      // this.$router.push({ name: 'brew' })
      // brew in progress (running, pause, error state)
      // this.$router.push('/progress')
    },
    gobook () {
      // this.$router.push('/book')
      this.$router.push({ name: 'book', params: { id: 'fromMain' } })
      // this.$router.push({ path: `/book/${'1'}` })
    },
    goclean () {
      this.$router.push('/clean')
    }
  }
}
</script>
