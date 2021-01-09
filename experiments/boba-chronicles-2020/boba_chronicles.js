
// var colorPalette = ["#8e7cbe", "#5eb3ea", "#26496d", "#5ddcb2", "#0c5f31", "#83ec66", "#36a620", "#a3c0a6", "#3330b7", "#f6adff", "#9d0d6c", "#f849b6", "#69345e", "#a261f6", "#c712e8", "#dfd945"];
// var colorPalette = ['#a68dc9', '#ad5e9d', '#6C5394', '#cc7fa8', '#f87574', '#d88c9a', '#f07a62', '#fcac62', '#fed37c', '#e8e180', '#bae485', '#97ce88', '#5f8a6e', '#7bbfb1', '#87e6e3', '#7bc5d8', '#6b9ac4', '#26496d'];
var colorPalette = [
'#a68dc9', '#7bc5d8', '#7bbfb1', '#ad5e9d', '#f87574', '#7bba6a',
'#6C5394', '#d88c9a', '#6b9ac4', '#5f8a6e', '#e8e180', '#26496d',
'#cc7fa8', '#fed37c', '#bae485', '#fff1b5', '#b4f0ee', '#fcac62'];

var priceColors = [
  '#5f8a6e',
  '#7b8f58',
  '#a48d48',
  '#d2834f',
  '#f87574',
];

var drinkColors = {
  'Milk': '#FDFFF5',
  'Milk Tea': '#c1905e',
  'Jasmine Milk Tea': '#f0c88c',
  'Oolong Milk Tea': '#b69675',
  'Strawberry Milk Tea': '#db9179',
  'Matcha': '#86945e',
  'Green Tea': '#BF6D24',
  'Black Tea': '#702f13',
  "Jasmine Tea": '#BC8238',
  'Fruit Tea': '#eeb835',
  'Strawberry Tea': '#C43709',
  'Ruby Tea': "#83282D",
};

var toppingColors = {
  'Tapioca': '#563e2c',
  'Mango': '#fe9408',
  'Grass Jelly': '#302e2c',
  'Lemon': '#f5d36b',
};

Vue.component('boba', {
    props: ['drink_color', 'topping_color',
            'Drink', 'Day', 'Store', 'City', 'ID'],
    template: `
    <div v-if="Day">
      <div v-bind:style="[drink_style, topping_style, border_style]" class="boba" v-if="$root.show_no_boba || ID">
        <div class="name" v-if="$root.show_name">{{ Drink }}</div>
        <div class="id_num" v-if="$root.show_id">{{ ID }}</div>
      </div>
    </div>
    <div v-else>
      <div class="boba empty" v-if="$root.show_no_boba"></div>
    </div>`,
    computed: {
      topping_style: function() {
        let color = toppingColors[this.topping_color];
        color = (typeof color !== 'undefined') ? color : 'transparent';
        return {'background-image': `linear-gradient(to bottom, transparent 80%, ${color} 80%)`}
      },
      border_style: function() {
        if (this.$root.border_select !== null && this.Drink) {
          if (this.$root.border_select === 'store') {
            return {'border-color': this.$root.stores[this.Store].color}
          }
          else if (this.$root.border_select === 'city') {
            return {'border-color': this.$root.cities[this.City].color}
          }
        }
        return {'border-color': 'white'}
      }
    },
    data: function() {
      return {
        drink_style: {
          'background': drinkColors[this.drink_color],
        },
      }
    }
})



var app = new Vue({
  el: '#app',
  data: {
    months: drinks,
    show_date: true,
    show_id: true,
    show_name: true,
    show_no_boba: true,
    border_select: null,
    border_options: [
      { display: 'None', value: null },
      { display: 'Store', value: 'store' },
      { display: 'City', value: 'city' },
    ]
  },
  computed: {
    stores: function() {
      return collate_info('Store');
      // let stores = [];
      // drinks.flat(2).forEach(function(obj) {
      //   if (obj.Store !== undefined) {
      //     stores.push(obj.Store);
      //   }
      // });
      // let stores_count = {};
      // stores.forEach(function(store) {
      //   stores_count[store] = (stores_count[store] || 0) +1;
      // });
      // let stores_info = {};
      // let color_ind = 0;
      // for (const store_name in stores_count) {
      //   let color = '#cecece';
      //   if (stores_count[store_name] > 1) {
      //     color = colorPalette[color_ind];
      //     color_ind = color_ind + 1;
      //   }
      //   stores_info[store_name] = {
      //     count: stores_count[store_name],
      //     color: color,
      //   }
      // }
      // return stores_info;
    },
    cities: function() {
      return collate_info('City');
    }
  }
})


function collate_info(key) {
  let occurances = [];
  drinks.flat(2).forEach(function(drink) {
    if (drink[key] !== undefined) {
      occurances.push(drink[key]);
    }
  });
  let count = {};
  occurances.forEach(function(x) {
    count[x] = (count[x] || 0) +1;
  });
  let info = {};
  let color_ind = 0;
  for (const name in count) {
    let color = '#cecece';
    color = colorPalette[color_ind];
    color_ind = color_ind + 1;
    // if (count[name] > 1) {
    //   color = colorPalette[color_ind];
    //   color_ind = color_ind + 1;
    // }
    info[name] = {
      count: count[name],
      color: color,
    }
  }
  return info;
}