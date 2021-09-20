<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <!-- <v-responsive :style="{ background: `rgb(${red}, ${green}, ${blue})` }" height="300px"></v-responsive> -->
          <v-card-text>
            <v-container fluid grid-list-lg>
              <v-layout row wrap>
                <v-flex xs9>
                  <h1> Brewery Setup</h1>
                </v-flex>
                <v-flex xs3>
                  <v-icon large color="orange darker-2" @click="gohome">home</v-icon>
                </v-flex>
                <v-flex xs6>
                  <v-text-field v-model="kettlevolume" :max="100" label="Kettle Volume" class="mt-0" type="number" hint="L" persistent-hint></v-text-field>
                </v-flex>
                <v-flex xs6>
                  <v-text-field v-model="mashtunvolume" :max="100" label="Mash Tun Volume" class="mt-0" type="number" hint="L" persistent-hint></v-text-field>
                </v-flex>
                <v-flex xs6>
                  <v-checkbox v-model="pumpedtransfer" label="Pumped Transfer"></v-checkbox>
                </v-flex>
                <v-flex xs9>
                  <v-select :items="heatingmethods" filled label="Heating Method"></v-select>
                </v-flex>
                <v-flex xs9>
                  <v-select :items="coolingmethods" filled label="Cooling Method"></v-select>
                </v-flex>
                <v-flex xs9>
                  <h2>General Settings</h2>
                </v-flex>
                <v-flex xs9>
                  <v-switch v-model="units" true-value="Metric" false-value="Imperial" :label="`Units: ${units.toString()}`"></v-switch>
                </v-flex>
                <v-flex xs6>
                  <v-text-field v-model="leadtime" :max="20" label="Lead Time for User Interaction" class="mt-0" type="number" hint="min" persistent-hint></v-text-field>
                </v-flex>
              </v-layout>
            </v-container>
          </v-card-text>
          <v-btn>Save</v-btn>
          <v-btn @click="gohome">Cancel</v-btn>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>


<script>

export default {
  data () {
    return {
      kettlevolume: 0,
      mashtunvolume: 0,
      pumpedtransfer: true,
      units: 'metric',
      leadtime: 5,
      heatingmethods: ['None', 'RIMS', 'Element'],
      coolingmethods: ['None', 'Wort Recirc', 'Water Recirc'] }
  },
  methods: {
    gohome () {
      this.$router.push('/')
    }
  }
}
</script>
