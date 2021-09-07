<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-responsive :style="{ background: `rgb(${red}, ${green}, ${blue})` }" height="300px"></v-responsive>
          <v-card-text>
            <v-container fluid grid-list-lg>
              <v-layout row wrap>
                <v-flex xs9>
                  <v-slider v-model="targettemp" :max="255" label="Target Temp (Sensor 1)"></v-slider>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="targettemp" class="mt-0" type="number"></v-text-field>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="currenttemp" label = "Current Temp" class="mt-0" type="number" value="0" readonly outlined></v-text-field>
                  
                </v-flex>
                <v-flex xs9>
                  <chart></chart>
                </v-flex>
                <v-flex xs9>
                  <v-slider v-model="red" :max="255" label="Red"></v-slider>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="red" class="mt-0" type="number"></v-text-field>
                </v-flex>
                <v-flex xs9>
                  <v-slider v-model="green" :max="255" label="G"></v-slider>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="green" class="mt-0" type="number"></v-text-field>
                </v-flex>
                <v-flex xs9>
                  <v-slider v-model="blue" :max="255" label="B"></v-slider>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="blue" class="mt-0" type="number"></v-text-field>
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
import Chart from './Chart.vue';


export default {
  data() {
    return { targettemp: 0, red: 160, green: 160, blue: 160 };
  },
  methods: {
    set_color: function() {
      this.$ajax
        .post("/api/v1/manual/brightness", {
          targettemp: this.targettemp,
          red: this.red,
          green: this.green,
          blue: this.blue
        })
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.log(error);
        });
    }
  },
  components: {
    Chart
   
  }
};
</script>
