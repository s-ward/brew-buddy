<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card color="orange accent-4">
          <v-img :src="require('../assets/logo.png')" contain height="200"></v-img>
          <v-img src="https://www.avcl.co.nz/COMP702ENEL791BrewBuddy/App/AppTest2/src/assets/logowhite.png" contain height="200"></v-img>
          <v-btn>Brew</v-btn>
          <v-btn @click="gosetup"><v-icon>home</v-icon>Brewery Setup</v-btn>
          <v-btn @click="gomanual">Manual Control</v-btn>
          <v-btn>Recipe Book</v-btn>
          <v-btn>Clean</v-btn>
          <v-card-title primary-title>
            <div class="ma-auto">
              <span class="grey--text">IDF version: {{version}}</span>
              <br>
              <span class="grey--text">ESP cores: {{cores}}</span>
            </div>
          </v-card-title>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
export default {
  data() {
    return {
      version: null,
      cores: null
    };
  },
  mounted() {
    this.$ajax
      .get("/api/v1/system/info")
      .then(data => {
        this.version = data.data.version;
        this.cores = data.data.cores;
      })
      .catch(error => {
        console.log(error);
      });
  },
  methods : {
     gomanual () {
  this.$router.push("/manual");
  },
  gosetup () {
    this.$router.push("/setup");
  }
  }
};
</script>
