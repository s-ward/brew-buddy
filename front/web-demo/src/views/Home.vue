<template color="blue accent-4">
  <v-container color="blue accent-4">
    <v-layout text-xs-center wrap color="green accent-4">
      <v-flex xs12 sm6 offset-sm3 color="purple accent-4">
        <v-card color="orange accent-4">
          <v-img :src="require('../assets/logo.png')" contain height="200"></v-img>
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
  }
};
</script>
