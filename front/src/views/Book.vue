<template>
  <v-card height="100%" class="fill-height justify-center text-center" color="orange lighten-3" elevation="0">
    <v-card-title class="text-h5 white--text">
      Recipe Book
      <!-- <span class="text-h5 white--text">Recipe Book</span> -->
      <v-spacer></v-spacer>
      <v-btn @click="gohome" icon><v-icon large color="orange accent-3">home</v-icon></v-btn>
    </v-card-title>

    <hr style="height:2px; width:90%; border-width:0; color:yellow; margin:5%; margin-top:0%; background-color:orange">

    <!-- <v-card-text class="text-left"> -->
      <!-- <p>JSON : {{selected}}</p> -->

      <template>
          <v-toolbar
            flat
            color="orange lighten-3"
            class="mx-0 px-0"
          >
            <v-dialog
              v-model="dialog"
              max-width="500px"
            >
              <template v-slot:activator="{ on, attrs }" class="mx-0 px-0">
                <v-text-field
                  v-model="search"
                  append-icon="search"
                  label="Search"
                  single-line
                  outlined
                  dense
                  hide-details
                  rounded
                ></v-text-field>
                <!-- <template v-slot:append-outer> -->
                <v-btn
                  @click="setview"
                  v-bind="attrs"
                  v-on="on"
                  text
                  dense
                >
                  <v-icon>add</v-icon>
                  New Recipe
                </v-btn>
                <!-- </template> -->
                <!-- </v-text-field> -->
              </template>
              <v-card>
                <!-- <v-card-title>
                  <span class="text-h5">{{ formTitle }}</span>
                </v-card-title> -->
                <v-card-title>
                  <v-text-field
                    outlined
                    v-model="editedItem.name"
                    label="Recipe Name"
                    :disabled="viewonly ? true : false"
                    :rules="[rules.required]"
                    counter
                    maxlength="20"
                  ></v-text-field>
                </v-card-title>
                <v-card-subtitle>Mash</v-card-subtitle>

                <v-card-text>
                  <v-container>
                    <v-row>
                      <v-col>
                        <!-- <v-text-field
                          v-model="editedItem.name"
                          label="Recipe Name"
                          :disabled="viewonly ? true : false"
                        ></v-text-field> -->
                        <v-text-field
                          v-model.number="editedItem.grainweight"
                          label="Grain Weight"
                          :disabled="viewonly ? true : false"
                          outlined
                          dense
                          type="number"
                        ></v-text-field>
                      </v-col>
                      <v-col>
                        <v-text-field
                          v-model.number="editedItem.mashwatervol"
                          label="Water Volume"
                          :disabled="viewonly ? true : false"
                          outlined
                          dense
                          type="number"
                        ></v-text-field>
                      </v-col>
                    </v-row>
                    <v-row>
                      <v-col>
                        <v-text-field
                          v-model.number="editedItem.striketemp"
                          label="Strike Temperature"
                          :disabled="viewonly ? true : false"
                          outlined
                          dense
                          type="number"
                        ></v-text-field>
                      </v-col>
                      <v-col>
                        <v-text-field
                          v-model.number="editedItem.flowrate"
                          label="Flow Rate"
                          :disabled="viewonly ? true : false"
                          outlined
                          dense
                          type="number"
                        ></v-text-field>
                      </v-col>
                    </v-row>
                    <!-- mash stuff in here somewhere -->
                    <v-data-table
                      :headers="mashstageheaders"
                      :items="editedItem.mashstages"
                      dense
                      hide-default-footer
                      :mobile-breakpoint='0'
                    >
                    <!-- remove padding from colummns, and change time and temp to text fields for inlien editing -->
                    <template v-slot:item="{item} ">
                      <tr>
                        <td class="mx-0 px-0">{{item.id}}</td>
                        <td class="mx-0 px-0">
                          <v-text-field
                            class="mx-0 px-0"
                            :disabled="viewonly ? true : false"
                            v-model.number="item.time"
                            solo
                            flat
                            :hide-details="true"
                            dense
                            single-line
                            type="number"
                          ></v-text-field>
                        </td>
                        <td class="mx-0 px-0">
                          <v-text-field
                            class="mx-0 px-0"
                            :disabled="viewonly"
                            v-model.number="item.temp"
                            solo
                            flat
                            :hide-details="true"
                            dense
                            single-line
                            type="number"
                          ></v-text-field>
                        </td>
                      </tr>
                    </template>
                      <!-- <template v-slot:item.time="{ item }" >
                        <v-text-field class="mx-0 px-0" :disabled="viewonly ? true : false" v-model="item.time" solo flat :hide-details="true" dense single-line></v-text-field>
                      </template> -->
                      <!-- <template v-slot:[`item.temp`]="{ item }">
                        <v-text-field class="mx-0 px-0" :disabled="viewonly" v-model="item.temp" solo flat :hide-details="true" dense single-line></v-text-field>
                      </template> -->
                    </v-data-table>
                  </v-container>
                </v-card-text>

                <v-card-subtitle>Sparge</v-card-subtitle>

                <v-card-text>
                  <v-container>
                    <v-row>
                      <v-col>
                        <v-text-field
                          v-model.number="editedItem.spargewatervol"
                          label="Water Volume"
                          :disabled="viewonly ? true : false"
                          outlined
                          dense
                          type="number"
                        ></v-text-field>
                      </v-col>
                      <v-col>
                        <v-text-field
                          v-model.number="editedItem.spargetemp"
                          label="Temperature"
                          :disabled="viewonly ? true : false"
                          outlined
                          dense
                          type="number"
                        ></v-text-field>
                      </v-col>
                    </v-row>
                  </v-container>
                </v-card-text>

                <v-card-subtitle>Boil</v-card-subtitle>

                <v-card-text>
                  <v-container>
                    <v-row>
                      <v-col>
                        <v-text-field
                          v-model.number="editedItem.boiltime"
                          label="Boil Time"
                          :disabled="viewonly ? true : false"
                          outlined
                          dense
                          type="number"
                        ></v-text-field>
                      </v-col>
                    </v-row>
                    <!-- adjunct stuff in here somewhere -->
                    <v-data-table
                      :headers="adjunctheaders"
                      :items="editedItem.adjuncts"
                      dense
                      hide-default-footer
                      :mobile-breakpoint='0'
                    >
                    <!-- remove padding from colummns, and change time and temp to text fields for inlien editing -->
                    <template v-slot:item="{item} ">
                      <tr>
                        <td class="mx-0 px-0">{{item.id}}</td>
                        <td class="mx-0 px-0">
                          <v-text-field
                            class="mx-0 px-0"
                            :disabled="viewonly ? true : false"
                            v-model.number="item.time"
                            solo
                            flat
                            :hide-details="true"
                            dense
                            single-line
                            type="number"
                          ></v-text-field>
                        </td>
                        <td class="mx-0 px-0">
                          <v-text-field
                            class="mx-0 px-0"
                            label="Adjuncts"
                            :disabled="viewonly"
                            v-model="item.adjuncts"
                            solo
                            flat
                            :hide-details="true"
                            dense
                            single-line
                          ></v-text-field>
                        </td>
                      </tr>
                    </template>
                      <!-- <template v-slot:item.time="{ item }" >
                        <v-text-field class="mx-0 px-0" :disabled="viewonly ? true : false" v-model="item.time" solo flat :hide-details="true" dense single-line></v-text-field>
                      </template> -->
                      <!-- <template v-slot:[`item.temp`]="{ item }">
                        <v-text-field class="mx-0 px-0" :disabled="viewonly" v-model="item.temp" solo flat :hide-details="true" dense single-line></v-text-field>
                      </template> -->
                    </v-data-table>

                    <!-- <v-card>
                      <v-row>
                        <v-col>
                          <v-text-field
                            v-model="editedItem.adjunctname"
                            label="Name"
                            :disabled="viewonly ? true : false"
                            outlined
                            dense
                          ></v-text-field>
                        </v-col>
                        <v-col>
                          <v-text-field
                            v-model="editedItem.adjuncttime"
                            label="Time"
                            :disabled="viewonly ? true : false"
                            outlined
                            dense
                          ></v-text-field>
                        </v-col>
                      </v-row>
                    </v-card> -->
                  </v-container>
                </v-card-text>

                <v-card-subtitle>Cool</v-card-subtitle>

                <v-card-text>
                  <v-container>
                    <v-row>
                      <v-col>
                        <v-text-field
                          v-model.number="editedItem.cooltemp"
                          label="Cooling Temperature"
                          :disabled="viewonly ? true : false"
                          outlined
                          dense
                          type="number"
                        ></v-text-field>
                      </v-col>
                    </v-row>
                  </v-container>
                </v-card-text>
                  <v-container fluid>
                    <v-row>
                      <v-col>
                        <v-textarea
                          outlined
                          v-model="editedItem.notes"
                          name="input-5-1"
                          label="Notes"
                          auto-grow
                          :disabled="viewonly ? true : false"
                          rows="2"
                        ></v-textarea>
                      </v-col>
                    </v-row>
                  </v-container>

                <v-card-actions>
                  <v-spacer></v-spacer>
                  <v-btn
                    color="blue darken-1"
                    text
                    @click="close"
                  >
                    Cancel
                  </v-btn>
                  <v-btn
                    v-if="!viewonly ? true : false"
                    :disabled="editedItem.name ? false : true"
                    color="blue darken-1"
                    text
                    @click="save"
                  >
                    Save
                  </v-btn>
                </v-card-actions>
              </v-card>
            </v-dialog>
            <v-dialog v-model="dialogDelete" max-width="500px">
              <v-card>
                <v-card-title class="text-h5">Are you sure you want to delete this item?</v-card-title>
                <v-card-actions>
                  <v-spacer></v-spacer>
                  <v-btn color="blue darken-1" text @click="closeDelete">Cancel</v-btn>
                  <v-btn color="blue darken-1" text @click="deleteItemConfirm">OK</v-btn>
                  <v-spacer></v-spacer>
                </v-card-actions>
              </v-card>
            </v-dialog>
          </v-toolbar>
        </template>

        <v-card-text class="mt-0 pt-0">

      <v-data-table
        :headers="headers"
        :items="recipes"
        class="elevation-1"
        hide-default-header
        hide-default-footer
        disable-pagination
        item-key="name"
        single-select
        v-model="selected"
        @click:row="rowClick"
        :search="search"
        :mobile-breakpoint='0'
      >
        <template v-slot:[`item.actions`]="{ item }">
          <v-icon
            small
            class="mr-2"
            @click="viewItem(item)"
        >
          remove_red_eye
          </v-icon>
          <v-icon
            small
            class="mr-2"
            @click="editItem(item)"
          >
            edit
          </v-icon>
          <v-icon
            small
            @click="deleteItem(item)"
          >
            delete
          </v-icon>
        </template>
      </v-data-table>
    </v-card-text>
<v-container>
    <v-row align="center" justify="space-around">
      <v-col align-self="center" class="mt-0 pt-0">
      <v-btn v-if="id === 'fromBrew'" :disabled="selected[0] ? false : true" @click="gobrew">Select Brew</v-btn>
    </v-col>
    </v-row>
    </v-container>
  </v-card>

</template>

<script>
export default {
  props: {
    id: {
      type: String,
      default: 'fromMain'
    }
  },
  data () {
    return {
      dialog: false,
      dialogDelete: false,
      selected: [],
      search: '',
      viewonly: 0,
      headers: [
        { text: 'Recipes', align: 'start', sortable: false, value: 'name' },
        { text: 'Actions', value: 'actions', sortable: false, align: 'end' }
      ],
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
      recipes: [],
      // mashstages: [{ id: 'Stage 11', time: 10, temp: 10 }, { id: 'Stage 22', time: 20, temp: 20 }],
      adjuncts: [],
      editedIndex: -1,
      editedItem: {
        name: '',
        grainweight: 0,
        mashwatervol: 0,
        striketemp: 0,
        flowrate: 0,
        // mashstages: [],
        mashstages: [
          { id: 'Stage 1', time: 0, temp: 0 },
          { id: 'Stage 2', time: 0, temp: 0 },
          { id: 'Stage 3', time: 0, temp: 0 },
          { id: 'Stage 4', time: 0, temp: 0 },
          { id: 'Stage 5', time: 0, temp: 0 }
        ],

        spargewatervol: 0,
        spargetemp: 0,
        boiltime: 0,
        // adjuncts: [],
        adjuncts: [
          { id: '1', time: 0 },
          { id: '2', time: 0 },
          { id: '3', time: 0 },
          { id: '4', time: 0 },
          { id: '5', time: 0 }
          // { id: '1', time: 0, adjuncts: 'Adjuncts' },
          // { id: '2', time: 0, adjuncts: 'Adjuncts' },
          // { id: '2', time: 0, adjuncts: 'Adjuncts' },
          // { id: '2', time: 0, adjuncts: 'Adjuncts' },
          // { id: '2', time: 0, adjuncts: 'Adjuncts' }
        ],
        cooltemp: 0,
        notes: ''
      },
      defaultItem: {
        name: '',
        grainweight: 0,
        mashwatervol: 0,
        striketemp: 0,
        flowrate: 0,
        mashstages: [
          { id: 'Stage 1', time: 0, temp: 0 },
          { id: 'Stage 2', time: 0, temp: 0 },
          { id: 'Stage 3', time: 0, temp: 0 },
          { id: 'Stage 4', time: 0, temp: 0 },
          { id: 'Stage 5', time: 0, temp: 0 }
        ],
        // mashstages: [{ id: 'Stage 1' }, { id: 'Stage 2' }, { id: 'Stage 3' }, { id: 'Stage 4' }, { id: 'Stage 5' }],
        time: 0,
        temp: 0,
        spargewatervol: 0,
        spargetemp: 0,
        boiltime: 0,
        adjuncts: [
          { id: '1', time: 0 },
          { id: '2', time: 0 },
          { id: '3', time: 0 },
          { id: '4', time: 0 },
          { id: '5', time: 0 }
          // { id: '1', time: 0, adjuncts: 'Adjuncts' },
          // { id: '2', time: 0, adjuncts: 'Adjuncts' },
          // { id: '2', time: 0, adjuncts: 'Adjuncts' },
          // { id: '2', time: 0, adjuncts: 'Adjuncts' },
          // { id: '2', time: 0, adjuncts: 'Adjuncts' }
        ],
        // adjuncts: [{ id: '1' }, { id: '2' }, { id: '3' }, { id: '4' }, { id: '5' }],
        adjunctname: '',
        adjuncttime: 0,
        cooltemp: 0,
        notes: ''
      },
      rules: {
        required: value => !!value || 'Required'
      }
    }
  },

  mounted () {
    this.$ajax
      .get('/api/v1/book/load')
      .then(data => {
        this.recipes = data.data.recipes
      })
      .catch(error => {
        console.log(error)
      })
  },

  watch: {
    dialog (val) {
      val || this.close()
    },
    dialogDelete (val) {
      val || this.closeDelete()
    }
  },

  methods: {
    rowClick: function (item, row) {
      let selectState = (row.isSelected) ? false : true
      row.select(selectState)
    },
    gohome () {
      this.$router.push('/')
    },
    gobrew () {
      // set recipe to selected, or pass recipe name? pass recipe info
      // stores selected recipe in store
      // this.$store.commit('set_RECIPE_JSON', )
      // this.$store.commit({ type: 'set_recipe', recipename: this.selected.name, grainweight: this.selected.grainweight })
      // selected is array containing one recipe, so selected recipe is at index 0
      this.$store.commit('set_recipe', { recipe: this.selected[0] })
      // this.$store.commit('set_recipe_array', this.selected)
      this.$router.push('/brew')
      // this.$router.push({ name: 'brew', params: { recipe: 'selected' } })
    },
    save_list: function () {
      this.$ajax
        .post('/api/v1/book/save', {
          recipes: this.recipes
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
    },
    setview () {
      this.viewonly = 0
    },
    viewItem (item) {
      this.viewonly = 1
      this.editedIndex = this.recipes.indexOf(item)
      this.editedItem = Object.assign({}, item)
      this.dialog = true
    },
    editItem (item) {
      this.viewonly = 0
      this.editedIndex = this.recipes.indexOf(item)
      this.editedItem = Object.assign({}, item)
      this.dialog = true
    },
    deleteItem (item) {
      this.editedIndex = this.recipes.indexOf(item)
      this.editedItem = Object.assign({}, item)
      this.dialogDelete = true
    },
    deleteItemConfirm () {
      this.recipes.splice(this.editedIndex, 1)
      this.save_list()
      this.closeDelete()
    },
    close () {
      this.dialog = false
      this.$nextTick(() => {
        this.editedItem = Object.assign({}, this.defaultItem)
        this.editedIndex = -1
      })
    },
    closeDelete () {
      this.dialogDelete = false
      this.$nextTick(() => {
        this.editedItem = Object.assign({}, this.defaultItem)
        this.editedIndex = -1
      })
    },
    save () {
      if (this.editedIndex > -1) {
        Object.assign(this.recipes[this.editedIndex], this.editedItem)
      } else {
        this.recipes.push(this.editedItem)
      }
      this.save_list()
      this.close()
    }
  }
}
</script>
