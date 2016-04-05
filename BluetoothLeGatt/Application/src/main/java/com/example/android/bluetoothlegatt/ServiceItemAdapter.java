package com.example.android.bluetoothlegatt;

import android.content.Context;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

/**
 * Created by Richard on 2016-04-04.
 */
public class ServiceItemAdapter extends RecyclerView.Adapter<ServiceItemAdapter.ViewHolder> {

    private Context context;

    List<ServiceDataItem> serviceItems;

    public  static  class ViewHolder extends RecyclerView.ViewHolder {
            private CardView cv;

            private String[] mDataset;

            private TextView title;
            private TextView value;
            private ImageButton btn;
            private ImageView image;

            public ViewHolder(View itemView) {
                super(itemView);
                cv = (CardView) itemView.findViewById(R.id.card_view);
                title = (TextView) itemView.findViewById(R.id.service_title);
                value = (TextView) itemView.findViewById(R.id.service_value);
                btn = (ImageButton) itemView.findViewById(R.id.detail_button);
                image = (ImageView) itemView.findViewById(R.id.image);
            }
        }

        ServiceItemAdapter (Context context,List<ServiceDataItem> dataItems) {
            this.serviceItems = dataItems;
            this.context = context;
        }
        @Override
        public void onBindViewHolder(final ViewHolder holder, final int position) {

            holder.title.setText(serviceItems.get(position).getTitle());
            holder.value.setText(serviceItems.get(position).getValue());
            holder.image.setImageResource(serviceItems.get(position).getImageId());
            Log.d("position", "" + position);
            holder.btn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Toast t = Toast.makeText(context, "Show details activity", Toast.LENGTH_LONG);
                    t.show();
                }
            });
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
