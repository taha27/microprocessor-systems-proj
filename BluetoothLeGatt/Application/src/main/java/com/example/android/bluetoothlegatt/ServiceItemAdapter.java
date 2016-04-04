package com.example.android.bluetoothlegatt;

import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.List;

/**
 * Created by Richard on 2016-04-04.
 */
public class ServiceItemAdapter extends RecyclerView.Adapter<ServiceItemAdapter.ViewHolder> {

        public  static  class ViewHolder extends RecyclerView.ViewHolder {
            private CardView cv;

            private String[] mDataset;

            private TextView title;
            private TextView value;

            public ViewHolder(View itemView) {
                super(itemView);
                cv = (CardView) itemView.findViewById(R.id.card_view);
                title = (TextView) itemView.findViewById(R.id.service_title);
                value = (TextView) itemView.findViewById(R.id.service_value);

            }
        }
        List<ServiceDataItem> serviceItems;

        ServiceItemAdapter (List<ServiceDataItem> dataItems) {
            this.serviceItems = dataItems;
        }
        @Override
        public void onBindViewHolder(ViewHolder holder, int position) {

            holder.title.setText(serviceItems.get(position).getTitle());
            holder.value.setText(serviceItems.get(position).getValue());
        }

        @Override
        public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.service_item, parent, false);
            ViewHolder viewHolder = new ViewHolder(v);
            return viewHolder;
        }

        @Override
        public void onAttachedToRecyclerView(RecyclerView recyclerView) {
            super.onAttachedToRecyclerView(recyclerView);
        }

        @Override
        public int getItemCount() {
            return serviceItems.size();
        }
}
