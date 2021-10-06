import Vue from 'vue'
import Vuex from 'vuex'
import axios from 'axios'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    chart_value: [8, 2, 5, 9, 5, 11, 3, 5, 10, 0, 1, 8, 2, 9, 0, 13, 10, 7, 16],
    ch_value: 10,
    temp1_chart_value: [8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8],
    temp1_value: 8,
    textarea: ['text line 1', 'text line 2', 'text line 3'],
    textarea2: [
      { textcolor: 'red--text', time: '44:10', text: 'Brew Paused: Please configure Mash Tun and confirm to continue' },
      { textcolor: 'green--text', time: '43:30', text: 'Brew Resumed: Mash Tun configuration confirmed' },
      { textcolor: 'yellow--text', time: '42:10', text: 'Brew Paused' },
      { textcolor: 'green--text', time: '40:15', text: 'Brew Resumed' },
      { textcolor: 'orange--text', time: '39:00', text: 'Please add Adjuncts 1 at time 34:00' },
      { textcolor: 'green--text', time: '33:55', text: 'Adjunct addition confirmed' }
    ],

    selectedrecipe: {
      name: '',
      grainweight: 0,
      mashwatervol: 0,
      striketemp: 0,
      flowrate: 0,
      mashstages: [],
      spargewatervol: 0,
      spargetemp: 0,
      boiltime: 0,
      adjuncts: [],
      cooltemp: 0,
      notes: ''
    },

    // state info
    brewstate: 3, // 3 is passive state, 2 manual state, 1 clean state, 4-9 brew states
    pauseint: 0, // 1 to tell esp to go to pause
    cancelint: 0, // 1 to tell esp to cancel
    // cleanint: 0, // 1 to tell esp to go to clean
    // brewint: 0, // 1 to tell esp to go to brew

    // brew progress info
    autoprocess: '',
    stage: '',
    step: '',
    status: 0, // 5 is out of range so nothing happens if brew is pressed before status is loaded
    minutesremaining: 11,
    secondsremaining: 33,
    userintreqmessage: '',
    userintreq: 0,
    adjunctreqmessage: '',
    adjunctreq: 0,

    // needs if etc.
    // statusinfo: {
    //   statusicon: 'pause_circle',
    //   statusiconcolor: 'green',
    //   statustextcolor: 'green--text'
    // }

  },
  mutations: {
    update_chart_value (state, new_value) {
      state.chart_value.push(new_value)
      state.chart_value.shift()
      state.ch_value = new_value
    },   
    update_temp1 (state, newTemp) {
      state.temp1_chart_value.push(newTemp)
      state.temp1_chart_value.shift()
      state.temp1_value = newTemp;
    },
    update_brew_progress (state, data) {
      state.brewstate = data.brewstate
      state.autoprocess = data.autoprocess
      // state.status = data.status
      state.minutesremaining = data.minutesremaining
      state.secondsremaining = data.secondsremaining
      state.autoprocess = data.autoprocess
      state.stage = data.stage
      state.step = data.step
      state.userintreq = data.userintreq
      state.userintreqmessage = data.userintreqmessage
      state.adjunctreq = data.adjunctreq
      // if message different to old message, push to array
      if (data.adjunctreq) {
        if (data.adjunctreqmessage !== state.adjunctreqmessage) {
          state.textarea.push(data.adjunctreqmessage)
          state.textarea.shift()
          state.adjunctreqmessage = data.adjunctreqmessage
        }
      }
    },
    set_recipe (state, recipe) {
      // do some shit with JSON array string etc?
      // needs recipe.recipe not just recipe.
      state.selectedrecipe.name = recipe.recipe.name
      state.selectedrecipe.grainweight = recipe.recipe.grainweight
      state.selectedrecipe.mashwatervol = recipe.recipe.mashwatervol
      state.selectedrecipe.striketemp = recipe.recipe.striketemp
      state.selectedrecipe.mashstages = recipe.recipe.mashstages
      state.selectedrecipe.flowrate = recipe.recipe.flowrate
      state.selectedrecipe.spargewatervol = recipe.recipe.spargewatervol
      state.selectedrecipe.spargetemp = recipe.recipe.spargetemp
      state.selectedrecipe.boiltime = recipe.recipe.boiltime
      state.selectedrecipe.adjuncts = recipe.recipe.adjuncts
      state.selectedrecipe.cooltemp = recipe.recipe.cooltemp
      state.selectedrecipe.notes = recipe.recipe.notes
    },
    set_recipe_JSON (state, recipeJSON) {
      state.recipeJSON = recipeJSON
    },
    add_adjunct_confirm () {
     
    },
    set_brew_state (state, brewstate) {
      state.brewstate = brewstate
      // console.log(brewstate)
      if (brewstate === 3) {
        state.status = 0
      } else if ([4, 5, 6, 7, 8, 9].includes(brewstate)) {
        state.status = 1
        // console.log(state.status)
      }
      // console.log(state.brewstate)
      // console.log(state.status)
    },
    set_brew_status (state, brewstatus) {
      state.status = brewstatus
    },
    add_message (state, message) {
      state.textarea2.push(message)
      // console.log(message)
    }

  },
  actions: {
    update_chart_value({ commit }) {
      axios.get("/api/v1/temp/raw")
        .then(data => {
          commit("update_chart_value", data.data.raw);
        })
        .catch(error => {
          console.log(error);
        });
    },
    update_temp1({ commit }) {
      axios.get("/api/v1/manual/raw")
        .then(data => {
          commit("update_temp1", data.data.raw);
        })
        .catch(error => {
          console.log(error);
        });
    },
    update_brewery_setup ({ commit }) {
      axios.get('/api/v1/setup/load')
        .then(data => {
          commit('update_chart_value', data.data.raw)
        })
        .catch(error => {
          console.log(error);
        });
    },
    update_brew_progress ({ commit }) {
      axios.get("/api/v1/progress/data")
        .then(data => {
          commit("update_brew_progress", data.data)
        })
        .catch(error => {
          console.log(error);
        });
    },
    // pause ({ commit }) {
      
    // },
    post_state_update ({ commit }) {
      // console.log(this.state.brewstate)
      axios.post('/api/v1/changestate', {
        brewstate: this.state.brewstate,
        pauseint: this.state.pauseint,
        cancelint: this.state.cancelint,
        // cleanint: this.state.cleanint, // maybe
        // brewint: this.state.brewint // maybe
      })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
    },
    set_brew_state_action ({ commit }, data) {
      return new Promise((resolve, reject) => {
        setTimeout(() => {
          commit('set_brew_state', data.brewstate)
          resolve()
          // console.log(data.brewstate)
        }, 1000)
      })
    }
    
    
  }
})