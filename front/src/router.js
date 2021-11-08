import Vue from 'vue'
import Router from 'vue-router'
import Home from './views/Home.vue'
import Chart from './views/Chart.vue'
import Light from './views/Light.vue'
import Manual from './views/Manual.vue'
import Book from './views/Book.vue'
import Brew from './views/Brew.vue'
import Clean from './views/Clean.vue'
import Progress from './views/Progress.vue'
import Recipe from './views/Recipe.vue'
import Setup from './views/Setup.vue'

Vue.use(Router)

export default new Router({
  mode: 'history',
  base: process.env.BASE_URL,
  routes: [
    {
      path: '/',
      name: 'home',
      component: Home
    },
    {
      path: '/chart',
      name: 'chart',
      component: Chart
    },
    {
      path: '/light',
      name: 'light',
      component: Light
    },
    {
      path: '/manual',
      name: 'manual',
      component: Manual
    },
    // {
    //   path: '/book',
    //   name: 'book',
    //   component: Book
    // },
    {
      path: '/book/:id',
      name: 'book',
      components: {
        default: Book
      },
      props: {
        default: true
      }
    },
    {
      path: '/brew',
      name: 'brew',
      component: Brew
    },
    // {
    //   path: '/brew/:recipe',
    //   name: 'brew',
    //   components: {
    //     default: Brew
    //   },
    //   props: {
    //     default: true
    //   }
    // },
    {
      path: '/clean',
      name: 'clean',
      component: Clean
    },
    {
      path: '/progress',
      name: 'progress',
      component: Progress
    },
    {
      path: '/recipe',
      name: 'recipe',
      component: Recipe
    },
    {
      path: '/setup/:id',
      name: 'setup',
      components: {
        default: Setup
      },
      props: {
        default: true
      }
    }
  ]
})
