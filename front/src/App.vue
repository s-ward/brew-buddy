








<template>
  <v-app id="inspire">
    <v-main class="grey lighten-3">
      <v-container fluid fill-height justify-center class="pa-0 ma-0">
        <v-col cols="12" sm="8"  md="6" lg="4" class="fill-height">
          <router-view ></router-view>
        </v-col>
      </v-container>
    </v-main>
  </v-app>
</template>

<script>
export default {
  name: 'App',
  data () {
    return {
      drawer: null
    }
  }
}
</script>
