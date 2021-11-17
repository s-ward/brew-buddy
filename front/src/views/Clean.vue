<template>
  <v-card height="100%" class="fill-height justify-center text-center" color="orange lighten-3" elevation="0">
    <v-card-title class="text-h5 white--text">
      Clean
      <v-spacer></v-spacer>
      <v-btn @click="gohome" color="orange accent-3" icon><v-icon large>home</v-icon></v-btn>
    </v-card-title>
    <hr style="height:2px; width:90%; border-width:0; color:yellow; margin:5%; margin-top:0%; background-color:orange">
    <v-card-text>
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
  </v-card>
</template>

<script>

export default {

  methods: {
    gohome () {
      this.$router.push('/')
    },
    fullclean () {
      this.$store.dispatch('set_brew_state_action', {
        brewstate: 4,
        pauseint: this.$store.state.pauseint,
        cancelint: this.$store.state.cancelint })
        .then(() => this.$store.dispatch('post_state_update')
          .then(() => this.$router.push('/progress')))
    },
    rinse () {
      this.$store.dispatch('post_state_update', {
        brewstate: 4,
        pauseint: this.$store.state.pauseint,
        cancelint: this.$store.state.cancelint })
        .then(() => this.$router.push('/progress'))
    }
  }
}

</script>
