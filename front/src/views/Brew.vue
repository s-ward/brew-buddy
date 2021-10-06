<template>
  <!-- <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-card-text>
            <v-container fluid grid-list-lg>
              <v-layout row wrap>
                <v-flex xs9>
                  <h1> Brew</h1>
                </v-flex>
                <v-flex xs3>
                  <v-icon large color="orange darker-2" @click="gohome">home</v-icon>
                </v-flex>
                <v-flex xs9>
                  <h2>Brewery Setup</h2>
                </v-flex>
                <v-flex xs3>
                  <v-icon small color="orange darker-2" @click="gosetup">settings</v-icon>
                </v-flex>
                <v-flex xs6>
                  <v-text-field v-model.number="kettlevolume" readonly :max="100" :min="0" label="Kettle Volume" class="mt-0" type="number" hint="L" persistent-hint></v-text-field>
                </v-flex>
                <v-flex xs6>
                  <v-text-field v-model.number="mashtunvolume" disabled :max="100" :min="0" label="Mash Tun Volume" class="mt-0" type="number" hint="L" persistent-hint></v-text-field>
                </v-flex>
                <v-flex xs6>
                  <v-checkbox v-model="pumpedtransfer" disabled label="Pumped Transfer"></v-checkbox>
                </v-flex>
                <v-flex xs9>
                  <v-select v-model="heatingmethod" disabled :items="heatingmethods" filled label="Heating Method"></v-select>
                </v-flex>
                <v-flex xs9>
                  <v-select v-model="coolingmethod" disabled :items="coolingmethods" filled label="Cooling Method"></v-select>
                </v-flex>
                <v-flex xs9>
                  <h2>Recipe</h2>
                </v-flex>
                <v-flex xs3>
                  <v-icon small color="orange darker-2" @click="gobook">book</v-icon>
                </v-flex>
                <v-flex xs9>

                </v-flex>
                <v-flex xs6>

                </v-flex>
              </v-layout>
            </v-container>
          </v-card-text>
          <v-btn @click="save_setup">Save</v-btn>
          <v-btn @click="gohome">Cancel</v-btn>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container> -->
  <v-card height="100%" class="fill-height justify-center text-center" color="orange lighten-3" elevation="0">
    <v-card-title class="text-h5 white--text"> Brew
      <!-- <span class="text-h5 white--text">Brew</span> -->
      <v-spacer></v-spacer>
      <v-btn @click="gohome" icon><v-icon large color="orange accent-3">home</v-icon></v-btn>  
    </v-card-title>


    <hr style="height:2px; width:90%; border-width:0; color:yellow; margin:5%; margin-top:0%; background-color:orange">
    <v-card-title class="text-h6 white--text mt-0 pt-0">
      Brewery Setup
      <v-spacer></v-spacer>
      <v-btn @click="gosetup" icon><v-icon color="orange accent-3">settings</v-icon></v-btn>
    </v-card-title>

    <v-card-text>
      <v-row>
        <v-col class="mb-0 pb-0">
          <v-text-field v-model.number="kettlevolume" disabled :max="100" :min="0" dense hide-details outlined label="Kettle Volume" class="mt-0" type="number" suffix="L"></v-text-field>
        </v-col>
        <v-col class="mb-0 pb-0">
          <v-text-field v-model.number="mashtunvolume" disabled :max="100" :min="0" dense hide-details outlined label="Mash Tun Volume" class="mt-0" type="number" suffix="L"></v-text-field>
        </v-col>
      </v-row>
      <v-row >
          <v-col class="my-0 py-0">
            <v-checkbox
              v-model="pumpedtransfer"
              label="Pumped Transfer"
              :on-icon="'check_circle_outline'"
              :off-icon="'highlight_off'"
              color="green"
              disabled
              hide-details
              dense
            ></v-checkbox>
          </v-col>
      </v-row>

      <v-row>
        <v-col>
          <v-select v-model="heatingmethod" :items="heatingmethods" dense hide-details disabled outlined label="Heating Method"></v-select>
        </v-col>
        <v-col>
          <v-select v-model="coolingmethod" :items="coolingmethods" dense hide-details disabled outlined label="Cooling Method"></v-select>
        </v-col>
      </v-row>
    </v-card-text>

    <v-card-title class="mt-0 pt-0">
      <span class="text-h6 white--text">Recipe</span>
      <v-spacer></v-spacer>
      <v-btn  @click="gobook" icon><v-icon color="orange accent-3">book</v-icon></v-btn>
    </v-card-title>
            <!-- <v-card> -->
              <v-template v-if="name !== ''">
    <v-card-text class="text-left">
      <v-text-field
        outlined
        v-model="name"
        label="Recipe Name"
        disabled
        hide-details
      ></v-text-field>
      <v-card-subtitle>Mash</v-card-subtitle>

      <!-- <v-card-text> -->
      <!-- <v-container> -->
      <v-row>
        <v-col>
          <!-- <v-text-field
            v-model="name"
            label="Recipe Name"
            disabled
          ></v-text-field> -->
          <v-text-field
            v-model="grainweight"
            label="Grain Weight"
            disabled
            outlined
            hide-details
            dense
          ></v-text-field>
        </v-col>
        <v-col>
          <v-text-field
            v-model="mashwatervol"
            label="Water Volume"
            disabled
            outlined
            hide-details
            dense
          ></v-text-field>
        </v-col>
      </v-row>
      <v-row>
        <v-col>
          <v-text-field
            v-model="striketemp"
            label="Strike Temperature"
            disabled
            outlined
            hide-details
            dense
          ></v-text-field>
        </v-col>
        <v-col>
          <v-text-field
            v-model="flowrate"
            label="Flow Rate"
            disabled
            outlined
            hide-details
            dense
          ></v-text-field>
        </v-col>
      </v-row>
      <!-- mash stuff in here somewhere -->
      <v-data-table
        :headers="mashstageheaders"
        :items="mashstages"
        dense
        hide-default-footer
        :mobile-breakpoint='0'
        class="orange lighten-3"
      >
      <!-- remove padding from colummns, and change time and temp to text fields for inlien editing -->
      <template v-slot:item="{item} ">
        <tr>
          <td class="mx-0 px-0">{{item.id}}</td>
          <td class="mx-0 px-0">
            <v-text-field class="mx-0 px-0" background-color="orange lighten-3" orange-lighten-3 disabled v-model="item.time" solo flat :hide-details="true" dense single-line></v-text-field>
          </td>
          <td class="mx-0 px-0">
            <v-text-field class="mx-0 px-0" background-color="orange lighten-3" color:orange lighten-3 disabled v-model="item.temp" solo flat :hide-details="true" dense single-line></v-text-field>
          </td>
        </tr>
      </template>
        <!-- <template v-slot:item.time="{ item }" >
          <v-text-field class="mx-0 px-0" disabled v-model="item.time" solo flat :hide-details="true" dense single-line></v-text-field>
        </template> -->
        <!-- <template v-slot:[`item.temp`]="{ item }">
          <v-text-field class="mx-0 px-0" disabled v-model="item.temp" solo flat :hide-details="true" dense single-line></v-text-field>
        </template> -->
      </v-data-table>
      <!-- </v-container> -->
      <!-- </v-card-text> -->

      <v-card-subtitle>Sparge</v-card-subtitle>

      <!-- <v-card-text> -->
      <!-- <v-container> -->
      <v-row>
        <v-col>
          <v-text-field
            v-model="spargewatervol"
            label="Water Volume"
            disabled
            outlined
            hide-details
            dense
          ></v-text-field>
        </v-col>
        <v-col>
          <v-text-field
            v-model="spargetemp"
            label="Temperature"
            disabled
            outlined
            hide-details
            dense
          ></v-text-field>
        </v-col>
      </v-row>
      <!-- </v-container> -->
      <!-- </v-card-text> -->

      <v-card-subtitle>Boil</v-card-subtitle>

      <!-- <v-card-text> -->
      <!-- <v-container> -->
      <v-row>
        <v-col>
          <v-text-field
            v-model="boiltime"
            label="Boil Time"
            disabled
            outlined
            hide-details
            dense
          ></v-text-field>
        </v-col>
      </v-row>
      <!-- adjunct stuff in here somewhere -->
      <v-data-table
        :headers="adjunctheaders"
        :items="adjuncts"
        dense
        hide-default-footer
        :mobile-breakpoint='0'
        class="orange lighten-3"
      >
      <!-- remove padding from colummns, and change time and temp to text fields for inlien editing -->
      <template v-slot:item="{item} ">
        <tr>
          <td class="mx-0 px-0">{{item.id}}</td>
          <td class="mx-0 px-0">
            <v-text-field class="mx-0 px-0" background-color="orange lighten-3" color="orange lighten-3" disabled v-model="item.time" solo flat :hide-details="true" dense single-line></v-text-field>
          </td>
          <td class="mx-0 px-0">
            <v-text-field class="mx-0 px-0 orange lighten-3" background-color="orange lighten-3" color="orange lighten-3" label="Adjuncts" disabled v-model="item.adjuncts" solo flat :hide-details="true" dense single-line></v-text-field>
          </td>
        </tr>
      </template>
        <!-- <template v-slot:item.time="{ item }" >
          <v-text-field class="mx-0 px-0" disabled v-model="item.time" solo flat :hide-details="true" dense single-line></v-text-field>
        </template> -->
        <!-- <template v-slot:[`item.temp`]="{ item }">
          <v-text-field class="mx-0 px-0" disabled v-model="item.temp" solo flat :hide-details="true" dense single-line></v-text-field>
        </template> -->
      </v-data-table>

      <!-- <v-card>
        <v-row>
          <v-col>
            <v-text-field
              v-model="adjunctname"
              label="Name"
              disabled
              outlined
              dense
            ></v-text-field>
          </v-col>
          <v-col>
            <v-text-field
              v-model="adjuncttime"
              label="Time"
              disabled
              outlined
              dense
            ></v-text-field>
          </v-col>
        </v-row>
      </v-card> -->
        <!-- </v-container> -->
      <!-- </v-card-text> -->

      <v-card-subtitle v-card-subtitle>Cool</v-card-subtitle>

      <!-- <v-card-text> -->
      <!-- <v-container> -->
      <v-row>
        <v-col>
          <v-text-field
            v-model="cooltemp"
            label="Cooling Temperature"
            disabled
            outlined
            hide-details
            dense
          ></v-text-field>
        </v-col>
      </v-row>
      <!-- </v-container> -->
      <!-- </v-card-text> -->
      <!-- <v-container fluid> -->
      <v-row>
        <v-col>
          <v-textarea
            outlined
            v-model="notes"
            name="input-5-1"
            label="Notes"
            auto-grow
            disabled
            hide-details
          ></v-textarea>
        </v-col>
      </v-row>
             <!-- </v-container> -->
        <!-- </v-card> -->

    </v-card-text>
    </v-template>
      <v-container>
      <v-row align="center" justify="center">
        <v-col align-self="center" class="mt-0 pt-0">
          <v-btn :disabled="!this.name" @click="start_brew">Start Brew</v-btn>
        </v-col>
      </v-row>
       </v-container>
    <!-- </v-card-text> -->
  </v-card>

</template>

<script>

export default {
  props: {
    recipe: {
      type: String,
      default: ''
    }
  },
  data () {
    return {
      // brewery info
      kettlevolume: 0,
      mashtunvolume: 0,
      pumpedtransfer: false,
      heatingmethod: '',
      coolingmethod: '',
      heatingmethods: ['RIMS', 'Element'],
      coolingmethods: ['None', 'Wort Recirc', 'Water Recirc'],
      // selected recipe info
      name: this.$store.state.selectedrecipe.name,
      grainweight: this.$store.state.selectedrecipe.grainweight,
      mashwatervol: this.$store.state.selectedrecipe.mashwatervol,
      striketemp: this.$store.state.selectedrecipe.striketemp,
      mashstages: this.$store.state.selectedrecipe.mashstages,
      flowrate: this.$store.state.selectedrecipe.flowrate,
      spargewatervol: this.$store.state.selectedrecipe.spargewatervol,
      spargetemp: this.$store.state.selectedrecipe.spargetemp,
      boiltime: this.$store.state.selectedrecipe.boiltime,
      adjuncts: this.$store.state.selectedrecipe.adjuncts,
      cooltemp: this.$store.state.selectedrecipe.cooltemp,
      notes: this.$store.state.selectedrecipe.notes,

      mashstageheaders: [
        { text: '', align: 'start', sortable: false, value: 'id', width: '38%', class: 'mx-0 px-0' },
        { text: 'Time', align: 'start', sortable: false, value: 'time', class: 'mx-0 px-0' },
        { text: 'Temp', align: 'start', sortable: false, value: 'temp', class: 'mx-0 px-0' }
      ],
      adjunctheaders: [
        { text: '', align: 'start', sortable: false, value: 'id', width: '20%', class: 'mx-0 px-0' },
        { text: 'Time', align: 'start', sortable: false, value: 'time', width: '20%', class: 'mx-0 px-0' },
        { text: 'Adjuncts', align: 'start', sortable: false, value: 'adjuncts' }
      ],
    }



  },
  mounted () {
    this.$ajax
      .get('/api/v1/setup/load')
      .then(data => {
        this.kettlevolume = data.data.kettlevolume
        this.mashtunvolume = data.data.mashtunvolume
        this.pumpedtransfer = data.data.pumpedtransfer
        this.heatingmethod = data.data.heatingmethod
        this.coolingmethod = data.data.coolingmethod
        this.units = data.data.units
        this.leadtime = data.data.leadtime
      })
      .catch(error => {
        console.log(error)
      })
  },
  methods: {
    gohome () {
      this.$router.push('/')
    },
    // gobook () {
    //   this.$router.push('/book')
    // },
    gobook () {
    // this.$router.push('/book')
      this.$router.push({ name: 'book', params: { id: 'fromBrew' } })
    // this.$router.push({ path: `/book/${'1'}` })
    },
    gosetup () {
      this.$router.push({ name: 'setup', params: { id: 'fromBrew' } })
    },
    goprogress () {
      // set esp state to brew
      // send store recipe info to esp
      // set recipe to selected

     // start_brew ()

      this.$router.push('/progress')
    },
    save_setup: function () {
      this.$ajax
        .post('/api/v1/setup/save', {
          kettlevolume: this.kettlevolume,
          mashtunvolume: this.mashtunvolume,
          pumpedtransfer: this.pumpedtransfer,
          heatingmethod: this.heatingmethod,
          coolingmethod: this.coolingmethod,
          units: this.units,
          leadtime: this.leadtime
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
    },
    start_brew: function () {
      this.$store.dispatch('set_brew_state_action', { brewstate: 4 })
        .then(() => this.$store.dispatch('post_state_update')
          .then(this.$ajax
            .post('/api/v1/brew/start', {
            // recipename: this.$store.state.selectedrecipe.name
              grainweight: this.$store.state.selectedrecipe.grainweight,
              mashwatervol: this.$store.state.selectedrecipe.mashwatervol,
              striketemp: this.$store.state.selectedrecipe.striketemp,
              mashstages: this.$store.state.selectedrecipe.mashstages,
              flowrate: this.$store.state.selectedrecipe.flowrate,
              spargewatervol: this.$store.state.selectedrecipe.spargewatervol,
              spargetemp: this.$store.state.selectedrecipe.spargetemp,
              boiltime: this.$store.state.selectedrecipe.boiltime,
              adjuncts: this.$store.state.selectedrecipe.adjuncts,
              cooltemp: this.$store.state.selectedrecipe.cooltemp
            })
            .then(data => {
              console.log(data)
            })
            .catch(error => {
              console.log(error)
            }))
          .then(this.$router.push('/progress')))
    }
    // get_selected_recipe: function () {

    //   this.name = this.$store.state.selectedrecipe.name,
    //   this.grainweight = this.$store.state.selectedrecipe.grainweight,
    //   this.mashwatervol = this.$store.state.selectedrecipe.mashwatervol,
    //   this.striketemp = this.$store.state.selectedrecipe.striketemp,
    //   this.time = this.$store.state.selectedrecipe.time,
    //   this.temp = this.$store.state.selectedrecipe.temp,
    //   this.flowrate = this.$store.state.selectedrecipe.flowrate,
    //   this.spargewatervol = this.$store.state.selectedrecipe.spargewatervol,
    //   this.spargetemp = this.$store.state.selectedrecipe.spargetemp,
    //   this.boiltime = this.$store.state.selectedrecipe.boiltime,
    //   this.adjunctname = this.$store.state.selectedrecipe.adjunctname,
    //   this.adjuncttime = this.$store.state.selectedrecipe.adjuncttime,
    //   this.cooltemp = this.$store.state.selectedrecipe.cooltemp,
    //   this.notes = this.$store.state.selectedrecipe.notes
    // }

  },
  computed: {
    get_recipe () {
      // this.currenttemp = this.$store.state.temp1_value;
      // this.currentflow = this.$store.state.temp1_value;
      return this.$store.state.temp1_value;
      //return this.$store.state.temp1_chart_value;
    }
  }
}
</script>
