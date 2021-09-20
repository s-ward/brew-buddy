<template color="green accent-4">
  <v-app id="inspire" color="blue accent-4">
    <v-navigation-drawer v-model="drawer" fixed app clipped>
      <v-list dense>
        <v-list-tile to="/">
          <v-list-tile-action>
            <v-icon>home</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Home</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile to="/chart">
          <v-list-tile-action>
            <v-icon>show_chart</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Cart</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile to="/light">
          <v-list-tile-action>
            <v-icon>highlight</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Light</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile to="/manual">
          <v-list-tile-action>
            <v-icon>highlight</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Manual Control</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile to="/book">
          <v-list-tile-action>
            <v-icon>highlight</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Recipe Book</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile to="/brew">
          <v-list-tile-action>
            <v-icon>highlight</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Brew</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile to="/clean">
          <v-list-tile-action>
            <v-icon>highlight</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Clean</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile to="/progress">
          <v-list-tile-action>
            <v-icon>highlight</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Brew Progress</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile to="/recipe">
          <v-list-tile-action>
            <v-icon>highlight</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Recipe</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile to="/setup">
          <v-list-tile-action>
            <v-icon>highlight</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Brewery Setup</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
      </v-list>
    </v-navigation-drawer>
    <v-toolbar color="red accent-4" dark fixed app clipped-left>
      <v-toolbar-side-icon @click.stop="drawer = !drawer"></v-toolbar-side-icon>
      <v-toolbar-title>Brew Buddy</v-toolbar-title>
    </v-toolbar>
    <v-content color="indigo accent-4" class="red">
      <v-container fluid fill-height teal>
        <router-view class="#C96E12"></router-view>
      </v-container>
    </v-content>
    <v-footer color="red accent-4" app fixed>
      <span class="white--text">&copy; ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD. All rights reserved.</span>
    </v-footer>
  </v-app>
</template>

<script>
export default {
  name: "App",
  data() {
    return {
      drawer: null
    };
  }
};
</script>
